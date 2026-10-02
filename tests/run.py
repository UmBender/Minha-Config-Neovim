#!/usr/bin/env python3
"""Test runner for the C++ template library (lib/) and the Neovim helpers.

Stages:
  1. lint        every lib/<area>/<name>.cpp has a valid header, a test (tests/<area>/<name>.cpp)
                 and an example (examples/<area>/<name>.cpp); variants (<name>.<variant>.cpp) too,
                 except educational ones (<name>.edu.cpp), which run the normal test instead
  2. standalone  every template compiles alone (bits/stdc++.h + using namespace std + its Requires)
  3. tests       every tests/<area>/<name>.cpp compiles (-Werror, sanitizers, debug STL) and passes;
                 the normal test also runs against each <name>.edu.cpp
  4. examples    every example compiles the same way and prints its `Output:` for its `Input:`
  5. docs        tasks/Library/ (one page per structure, from templates + variants + examples) is up to date
  6. python      the Python library (pylib/<area>/<name>.py): lint, standalone exec, tests
                 (tests/py/<area>/<name>.py), examples (examples/py/<area>/<name>.py); plus the
                 Python file templates' tests (tests/py/templates/)
  7. nvim        tests/nvim/*_test.lua pass

Usage: tests/run.py [FILTER ...] [-j N] [--no-nvim] [--write-docs]
FILTER selects templates/tests whose id ("dsa/fenwick-tree", "py/gen/tree") contains it; "nvim" selects
the nvim tests, "py" every Python one.
--write-docs regenerates tasks/Library/ instead of checking it.
"""

import argparse
import ast
import concurrent.futures as cf
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LIB = ROOT / "lib"
TESTS = ROOT / "tests"
EXAMPLES = ROOT / "examples"
DOCS = ROOT / "tasks" / "Library"
BUILD = TESTS / ".build"
PYLIB = ROOT / "pylib"
PYTESTS = TESTS / "py"
PYEXAMPLES = EXAMPLES / "py"
PYDOCS = DOCS / "python"
PYTHON = [sys.executable, "-X", "dev", "-W", "error"]

CXX = os.environ.get("CXX", "g++")
WARN = ["-std=c++20", "-Wall", "-Wextra", "-Wshadow", "-Werror"]
TEST_FLAGS = WARN + [
    "-O1",
    "-g",
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-D_GLIBCXX_DEBUG",
    "-D_GLIBCXX_DEBUG_PEDANTIC",
    f"-I{LIB}",
    f"-I{TESTS / 'include'}",
]
TIMEOUT = 60
REQUIRED_KEYS = ["Title", "Description", "Usage", "Complexity"]
EXAMPLE_KEYS = ["Problem", "Output"]
EDU = "edu"
FORBIDDEN = [
    (re.compile(r"^\s*#\s*include\b"), "#include (templates must be self-contained)"),
    (re.compile(r"^\s*#\s*define\b"), "#define (no macros in templates)"),
    (re.compile(r"^\s*#\s*pragma\b"), "#pragma"),
    (re.compile(r"^\s*using\s+namespace\s+std\b"), "using namespace std (already in the solution file)"),
    (re.compile(r"\bassert\s*\("), "assert (GCC 16's bits/stdc++.h no longer includes <cassert>)"),
]

GREEN, RED, DIM, RESET = ("\033[32m", "\033[31m", "\033[2m", "\033[0m") if sys.stdout.isatty() else ("",) * 4


def lib_ids():
    """Templates and their variants: "dsa/segtree", "dsa/segtree.edu", "dsa/segtree.sum", ..."""
    return sorted(str(p.relative_to(LIB).with_suffix("")) for p in LIB.rglob("*.cpp"))


def base_of(tid):
    """"dsa/segtree.sum" -> "dsa/segtree" (a normal template is its own base)."""
    return tid.split(".")[0]


def variant_of(tid):
    """"dsa/segtree.sum" -> "sum", None for a normal template."""
    return tid.split(".", 1)[1] if "." in tid else None


def variants(tid):
    """Variants of a normal template: educational first, then common uses by name."""
    vs = [v for v in lib_ids() if base_of(v) == tid and v != tid]
    return sorted(vs, key=lambda v: (variant_of(v) != EDU, v))


def edu_ids():
    return [t for t in lib_ids() if variant_of(t) == EDU]


def test_ids():
    return sorted(
        str(p.relative_to(TESTS).with_suffix(""))
        for p in TESTS.rglob("*.cpp")
        if p.relative_to(TESTS).parts[0] not in (".build", "nvim", "include")
    )


def example_ids():
    return sorted(str(p.relative_to(EXAMPLES).with_suffix("")) for p in EXAMPLES.rglob("*.cpp"))


def parse_header(path, comment="//"):
    """Header = leading `// Key: value` comment block. Indented `//   ...` lines continue the previous key."""
    meta, key = {}, None
    for line in path.read_text().splitlines():
        if not line.startswith(comment):
            break
        body = line[len(comment):]
        m = re.match(r"^ (\w+):\s?(.*)$", body)
        if m:
            key = m.group(1)
            meta.setdefault(key, []).append(m.group(2).rstrip())
        elif key:
            meta[key].append(body.strip())
    return {k: "\n".join(v).strip() for k, v in meta.items()}


def requires(tid):
    req = parse_header(LIB / f"{tid}.cpp").get("Requires", "")
    return [r.strip() for r in re.split(r"[,\s]+", req) if r.strip()]


def resolve(tid, seen=None, order=None):
    """Dependencies first, each once."""
    seen = set() if seen is None else seen
    order = [] if order is None else order
    if tid in seen:
        return order
    seen.add(tid)
    for dep in requires(tid):
        resolve(dep, seen, order)
    order.append(tid)
    return order


def lint(ids, match=lambda tid: True):
    errors = []
    all_ids = set(lib_ids())
    for tid in ids:
        path = LIB / f"{tid}.cpp"
        meta = parse_header(path)
        for key in REQUIRED_KEYS:
            if not meta.get(key):
                errors.append(f"{tid}: missing header key '{key}'")
        var = variant_of(tid)
        if var is not None and base_of(tid) not in all_ids:
            errors.append(f"{tid}: variant of unknown template '{base_of(tid)}'")
        if any(re.match(r"^// Presets\b", l) for l in path.read_text().splitlines()):
            errors.append(f"{tid}: 'Presets' are gone, move them to variants (D-013)")
        if var == EDU:
            base = LIB / f"{base_of(tid)}.cpp"
            if base.exists() and meta.get("Title") != parse_header(base).get("Title"):
                errors.append(f"{tid}: an educational variant keeps the Title of {base_of(tid)}")
            for d in (TESTS, EXAMPLES):
                if (d / f"{tid}.cpp").exists():
                    errors.append(f"{d.name}/{tid}.cpp: educational variants use the normal test/example")
            continue
        if re.search(r'[\\/:#^\[\]|]', meta.get("Title", "")):
            errors.append(f"{tid}: Title can't contain \\ / : # ^ [ ] | (it names the docs page)")
        for dep in requires(tid):
            if dep not in all_ids:
                errors.append(f"{tid}: Requires unknown template '{dep}'")
        for n, line in enumerate(path.read_text().splitlines(), 1):
            for rx, what in FORBIDDEN:
                if rx.search(line):
                    errors.append(f"{tid}:{n}: forbidden {what}")
        if not (TESTS / f"{tid}.cpp").exists():
            errors.append(f"{tid}: no test (expected tests/{tid}.cpp)")
        ex = EXAMPLES / f"{tid}.cpp"
        if meta.get("Pending"):  # migration in progress: example not required yet
            if ex.exists():
                errors.append(f"{tid}: has an example but is still marked 'Pending:' (remove the marker)")
        elif not ex.exists():
            errors.append(f"{tid}: no example (expected examples/{tid}.cpp)")
        else:
            emeta = parse_header(ex)
            for key in EXAMPLE_KEYS:
                if not emeta.get(key):
                    errors.append(f"examples/{tid}.cpp: missing header key '{key}'")
            if f'#include "{tid}.cpp"' not in ex.read_text():
                errors.append(f'examples/{tid}.cpp: must #include "{tid}.cpp"')
    for tid in filter(match, test_ids()):
        if tid not in all_ids:
            errors.append(f"tests/{tid}.cpp: no matching template lib/{tid}.cpp")
    for tid in filter(match, example_ids()):
        if tid not in all_ids:
            errors.append(f"examples/{tid}.cpp: no matching template lib/{tid}.cpp")
    return errors


def standalone(tid):
    src = "#include <bits/stdc++.h>\nusing namespace std;\n"
    for dep in resolve(tid):
        src += (LIB / f"{dep}.cpp").read_text() + "\n"
    src += "int main() {}\n"
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write(src)
    try:
        r = subprocess.run([CXX, *WARN, "-fsyntax-only", f.name], capture_output=True, text=True)
    finally:
        os.unlink(f.name)
    return r.returncode == 0, r.stderr


def included_files(path, acc=None):
    acc = set() if acc is None else acc
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', path.read_text(), re.M):
        for base in (path.parent, LIB, TESTS / "include"):
            dep = (base / m.group(1)).resolve()
            if dep.exists():
                if dep not in acc:
                    acc.add(dep)
                    included_files(dep, acc)
                break
    return acc


def build_and_run(tid, src=None, stdin=None):
    """Builds and runs tests/<tid>.cpp (or `src`). For an educational variant "<base>.edu" it builds the
    normal test of <base> with `#include "<base>.cpp"` redirected to the .edu.cpp (an include overlay)."""
    flags, extra = TEST_FLAGS, []
    if src is None and variant_of(tid) == EDU:
        src = TESTS / f"{base_of(tid)}.cpp"
        overlay = BUILD / "overlay" / tid
        shim = overlay / f"{base_of(tid)}.cpp"
        shim.parent.mkdir(parents=True, exist_ok=True)
        text = f'#include "{LIB / tid}.cpp"\n'
        if not shim.exists() or shim.read_text() != text:
            shim.write_text(text)
        flags, extra = [f"-I{overlay}", *TEST_FLAGS], [LIB / f"{tid}.cpp"]
    src = src or TESTS / f"{tid}.cpp"
    h = hashlib.sha256(" ".join([CXX, *flags]).encode())
    for p in sorted({src.resolve(), *included_files(src), *extra}):
        h.update(str(p).encode() + p.read_bytes())
    exe = BUILD / (Path("edu") / tid if extra else src.relative_to(ROOT).with_suffix("")) / h.hexdigest()[:16]
    t0 = time.time()
    if not exe.exists():
        if exe.parent.exists():
            shutil.rmtree(exe.parent)
        exe.parent.mkdir(parents=True)
        r = subprocess.run([CXX, *flags, str(src), "-o", str(exe)], capture_output=True, text=True)
        if r.returncode != 0:
            return False, "compile error\n" + r.stderr, time.time() - t0
    try:
        r = subprocess.run(
            [str(exe)], input=stdin, capture_output=True, text=True, timeout=TIMEOUT, cwd=exe.parent
        )
    except subprocess.TimeoutExpired:
        return False, f"timeout after {TIMEOUT}s", time.time() - t0
    if stdin is not None:
        return r.returncode == 0, (r.stdout, r.stderr), time.time() - t0
    out = (r.stdout + r.stderr).strip()
    return r.returncode == 0, out, time.time() - t0


def normalize(text):
    lines = [l.rstrip() for l in text.strip().splitlines()]
    return "\n".join(lines)


def run_example(tid):
    path = EXAMPLES / f"{tid}.cpp"
    meta = parse_header(path)
    stdin = meta.get("Input", "") + "\n"
    ok, out, secs = build_and_run(tid, path, stdin)
    if isinstance(out, str):  # compile error / timeout
        return False, out, secs
    stdout, stderr = out
    if not ok:
        return False, (stdout + stderr).strip(), secs
    want, got = normalize(meta.get("Output", "")), normalize(stdout)
    if want != got:
        return False, f"wrong output\n--- want\n{want}\n--- got\n{got}", secs
    return True, "", secs


# ---------- docs (tasks/Library) ----------

def code_lines(path, comment="//"):
    """Source without the leading header comment block."""
    lines = path.read_text().splitlines()
    i = 0
    while i < len(lines) and lines[i].startswith(comment):
        i += 1
    while i < len(lines) and not lines[i].strip():
        i += 1
    return "\n".join(lines[i:]).rstrip()


def page_name(tid):
    return parse_header(LIB / f"{tid}.cpp")["Title"]


def usage_sections(tid, level):
    """Usage, complexity, example and judge links of one template or variant."""
    meta = parse_header(LIB / f"{tid}.cpp")
    h = "#" * level
    out = ["", f"{h} Usage", "", "```cpp", meta["Usage"], "```"]
    out += ["", f"{h} Complexity", "", meta["Complexity"]]
    ex = EXAMPLES / f"{tid}.cpp"
    if ex.exists():
        em = parse_header(ex)
        out += ["", f"{h} Example", "", em.get("Problem", ""), "", "```cpp", code_lines(ex), "```"]
        if em.get("Input"):
            out += ["", "Input:", "", "```", em["Input"], "```"]
        out += ["", "Output:", "", "```", em.get("Output", ""), "```"]
    verify = [v for v in meta.get("Verify", "").splitlines() if v.strip()]
    if verify:
        out += ["", f"{h} Verify", ""] + [f"- {v.strip()}" for v in verify]
    return out


def variant_label(tid):
    var = variant_of(tid)
    return "normal" if var is None else "educational" if var == EDU else var


def gen_page(tid):
    meta = parse_header(LIB / f"{tid}.cpp")
    area = tid.split("/")[0]
    vs = variants(tid)
    out = ["---", f"tags: [library, {area}]", "generated: true", "---", f"# {meta['Title']}", ""]
    srcs = " and ".join(f"`{d}/{tid}.cpp`" for d in ("lib", "examples")) + (" (plus variants)" if vs else "")
    out += ["> [!info] Generated by `tests/run.py --write-docs` from", f"> {srcs}. Edit those, not this page.", ""]
    pick = f"`{tid}` → normal" if vs else f"`{tid}`"
    out += [meta["Description"], "", f"Insert with `<leader>rl` → {pick}."]
    if requires(tid):
        out += ["Requires: " + ", ".join(f"[[{page_name(d)}]]" for d in requires(tid)) + " (inserted automatically)."]
    if vs:
        out += ["", "## Variants", "", "Chosen in the second `<leader>rl` menu.", ""]
        out += ["| Variant | Id | Description |", "| ------- | -- | ----------- |"]
        for v in [tid, *vs]:
            desc = parse_header(LIB / f"{v}.cpp")["Description"].replace("|", "\\|")
            link = f"[[#{parse_header(LIB / f'{v}.cpp')['Title']}]]" if variant_of(v) not in (None, EDU) else variant_label(v)
            out.append(f"| {link} | `{v}` | {desc} |")
    out += usage_sections(tid, 2)
    for v in vs:
        if variant_of(v) == EDU:
            continue
        vm = parse_header(LIB / f"{v}.cpp")
        out += ["", f"## {vm['Title']}", "", vm["Description"], "", f"Insert with `<leader>rl` → `{tid}` → {variant_of(v)}."]
        if requires(v):
            out += ["Requires: " + ", ".join(f"[[{page_name(d)}]]" for d in requires(v)) + " (inserted automatically)."]
        out += usage_sections(v, 3)
    return "\n".join(out) + "\n"


def gen_index():
    out = ["---", "tags: [library, index]", "generated: true", "---", "# Library", ""]
    out += ["Generated by `tests/run.py --write-docs`. One page per template in `lib/`; see [[Template Library]].", ""]
    by_area = {}
    for tid in filter(lambda t: variant_of(t) is None, lib_ids()):
        by_area.setdefault(tid.split("/")[0], []).append(tid)
    for area, tids in by_area.items():
        out += [f"## {area}", "", "| Template | Description | Variants |", "| -------- | ----------- | -------- |"]
        for tid in tids:
            meta = parse_header(LIB / f"{tid}.cpp")
            vs = ", ".join(variant_label(v) for v in variants(tid))
            out.append(f"| [[{meta['Title']}]] | {meta['Description'].replace('|', '\\|')} | {vs} |")
        out.append("")
    by_area = {}
    for tid in py_ids():
        by_area.setdefault(tid.split("/")[0], []).append(tid)
    for area, tids in by_area.items():
        out += [f"## python/{area}", "", "| Template | Description |", "| -------- | ----------- |"]
        for tid in tids:
            meta = py_meta(tid)
            out.append(f"| [[{meta['Title']}]] | {meta['Description'].replace('|', '\\|')} |")
        out.append("")
    return "\n".join(out)


def docs(ids, pyids, write, full):
    """Returns the stale/extra pages (or the written ones when write=True)."""
    bases = sorted({base_of(t) for t in ids if (LIB / f"{base_of(t)}.cpp").exists()})
    wanted = {DOCS / tid.split("/")[0] / f"{page_name(tid)}.md": gen_page(tid) for tid in bases}
    wanted.update({py_page_path(tid): gen_py_page(tid) for tid in pyids})
    if full:
        wanted[DOCS / "Library.md"] = gen_index()
    changed = [p for p, text in wanted.items() if not p.exists() or p.read_text() != text]
    extra = []
    if full:
        extra = [p for p in DOCS.rglob("*.md") if p not in wanted] if DOCS.exists() else []
    if write:
        for p in changed:
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(wanted[p])
        for p in extra:
            p.unlink()
    return changed, extra


# ---------- Python library (pylib/) ----------
# Same header format with `#` comments. No variants. Templates are pasted into a script, so they must be
# self-contained (stdlib imports inside the template) and have no top-level side effects.

PY_TEMPLATE_TESTS = "templates"  # tests/py/templates/: tests of templates/py/*.py, not of a pylib file


def py_ids():
    return sorted(str(p.relative_to(PYLIB).with_suffix("")) for p in PYLIB.rglob("*.py"))


def py_meta(tid):
    return parse_header(PYLIB / f"{tid}.py", "#")


def py_requires(tid):
    return [r for r in re.split(r"[,\s]+", py_meta(tid).get("Requires", "")) if r]


def py_resolve(tid, seen=None, order=None):
    seen = set() if seen is None else seen
    order = [] if order is None else order
    if tid not in seen:
        seen.add(tid)
        for dep in py_requires(tid):
            py_resolve(dep, seen, order)
        order.append(tid)
    return order


def py_test_ids():
    """tests/py/<area>/<name>.py, as "<area>/<name>" (the template tests included)."""
    return sorted(
        str(p.relative_to(PYTESTS).with_suffix("")) for p in PYTESTS.rglob("*.py") if p.parent.name != "include"
    )


def py_example_ids():
    return sorted(str(p.relative_to(PYEXAMPLES).with_suffix("")) for p in PYEXAMPLES.rglob("*.py"))


def py_problems(tid, path):
    """Imports outside the stdlib, `if __name__`, top-level statements with side effects."""
    errors = []
    try:
        tree = ast.parse(path.read_text())
    except SyntaxError as e:
        return [f"py/{tid}: syntax error: {e}"]
    for node in tree.body:
        if isinstance(node, (ast.Import, ast.ImportFrom)):
            names = [a.name for a in node.names] if isinstance(node, ast.Import) else [node.module or ""]
            for name in names:
                if name.split(".")[0] not in sys.stdlib_module_names:
                    errors.append(f"py/{tid}:{node.lineno}: imports '{name}' (stdlib only)")
        elif isinstance(node, ast.If) and "__name__" in ast.unparse(node.test):
            errors.append(f"py/{tid}:{node.lineno}: `if __name__` (the template is pasted into a script)")
        elif isinstance(node, ast.Expr) and not isinstance(node.value, ast.Constant):
            errors.append(f"py/{tid}:{node.lineno}: top-level expression (no side effects in templates)")
        elif isinstance(node, (ast.For, ast.While, ast.With, ast.Try)):
            errors.append(f"py/{tid}:{node.lineno}: top-level {type(node).__name__} (no side effects in templates)")
    return errors


def py_lint(ids, match):
    errors = []
    all_ids = set(py_ids())
    cpp_titles = {parse_header(LIB / f"{t}.cpp").get("Title") for t in lib_ids() if variant_of(t) is None}
    titles = {}
    for tid in py_ids():
        titles.setdefault(py_meta(tid).get("Title"), []).append(tid)
    for tid in ids:
        path = PYLIB / f"{tid}.py"
        meta = py_meta(tid)
        for key in REQUIRED_KEYS:
            if not meta.get(key):
                errors.append(f"py/{tid}: missing header key '{key}'")
        title = meta.get("Title", "")
        if "." in Path(tid).name:
            errors.append(f"py/{tid}: Python templates have no variants (no '.' in the name)")
        if re.search(r'[\\/:#^\[\]|]', title):
            errors.append(f"py/{tid}: Title can't contain \\ / : # ^ [ ] | (it names the docs page)")
        if title in cpp_titles or len(titles.get(title, [])) > 1:
            errors.append(f"py/{tid}: Title '{title}' is taken (docs pages are named by Title)")
        for dep in py_requires(tid):
            if dep not in all_ids:
                errors.append(f"py/{tid}: Requires unknown template '{dep}'")
        errors += py_problems(tid, path)
        if not (PYTESTS / f"{tid}.py").exists():
            errors.append(f"py/{tid}: no test (expected tests/py/{tid}.py)")
        ex = PYEXAMPLES / f"{tid}.py"
        if not ex.exists():
            errors.append(f"py/{tid}: no example (expected examples/py/{tid}.py)")
        else:
            emeta = parse_header(ex, "#")
            for key in EXAMPLE_KEYS:
                if not emeta.get(key):
                    errors.append(f"examples/py/{tid}.py: missing header key '{key}'")
            if f'include("{tid}")' not in ex.read_text():
                errors.append(f'examples/py/{tid}.py: must include("{tid}")')
    for tid in py_test_ids():
        if match("py/" + tid) and tid.split("/")[0] != PY_TEMPLATE_TESTS and tid not in all_ids:
            errors.append(f"tests/py/{tid}.py: no matching template pylib/{tid}.py")
    for tid in py_example_ids():
        if match("py/" + tid) and tid not in all_ids:
            errors.append(f"examples/py/{tid}.py: no matching template pylib/{tid}.py")
    return errors


def py_run(args, stdin=None):
    env = dict(os.environ, PYTHONPATH=str(PYTESTS / "include"), PYTHONDONTWRITEBYTECODE="1", PYTHON_COLORS="0")
    t0 = time.time()
    try:
        r = subprocess.run([*PYTHON, *args], input=stdin, capture_output=True, text=True, timeout=TIMEOUT, env=env)
    except subprocess.TimeoutExpired:
        return None, f"timeout after {TIMEOUT}s", time.time() - t0
    return r, "", time.time() - t0


def py_standalone(tid):
    """The template and its Requires, pasted into an empty script, run without errors or output."""
    src = "\n".join((PYLIB / f"{dep}.py").read_text() for dep in py_resolve(tid))
    r, err, _ = py_run(["-c", src])
    if r is None:
        return False, err
    if r.returncode != 0 or r.stdout or r.stderr:
        return False, (r.stdout + r.stderr).strip() or "printed something"
    return True, ""


def py_test(tid):
    r, err, secs = py_run([str(PYTESTS / f"{tid}.py")])
    if r is None:
        return False, err, secs
    return r.returncode == 0, (r.stdout + r.stderr).strip(), secs


def py_example(tid):
    path = PYEXAMPLES / f"{tid}.py"
    meta = parse_header(path, "#")
    r, err, secs = py_run([str(path)], meta.get("Input", "") + "\n")
    if r is None:
        return False, err, secs
    if r.returncode != 0:
        return False, (r.stdout + r.stderr).strip(), secs
    want, got = normalize(meta.get("Output", "")), normalize(r.stdout)
    if want != got:
        return False, f"wrong output\n--- want\n{want}\n--- got\n{got}", secs
    return True, "", secs


def gen_py_page(tid):
    meta = py_meta(tid)
    area = tid.split("/")[0]
    out = ["---", f"tags: [library, python, {area}]", "generated: true", "---", f"# {meta['Title']}", ""]
    out += ["> [!info] Generated by `tests/run.py --write-docs` from", f"> `pylib/{tid}.py` and `examples/py/{tid}.py`. Edit those, not this page.", ""]
    out += [meta["Description"], "", f"Insert with `<leader>rl` in a Python buffer → `{tid}`."]
    if py_requires(tid):
        out += ["Requires: " + ", ".join(f"[[{py_meta(d)['Title']}]]" for d in py_requires(tid)) + " (inserted automatically)."]
    out += ["", "## Usage", "", "```python", meta["Usage"], "```", "", "## Complexity", "", meta["Complexity"]]
    ex = PYEXAMPLES / f"{tid}.py"
    if ex.exists():
        em = parse_header(ex, "#")
        out += ["", "## Example", "", em.get("Problem", ""), "", "```python", code_lines(ex, "#"), "```"]
        if em.get("Input"):
            out += ["", "Input:", "", "```", em["Input"], "```"]
        out += ["", "Output:", "", "```", em.get("Output", ""), "```"]
    verify = [v for v in meta.get("Verify", "").splitlines() if v.strip()]
    if verify:
        out += ["", "## Verify", ""] + [f"- {v.strip()}" for v in verify]
    return "\n".join(out) + "\n"


def py_page_path(tid):
    return PYDOCS / tid.split("/")[0] / f"{py_meta(tid)['Title']}.md"


def nvim_tests():
    results = []
    for path in sorted((TESTS / "nvim").glob("*_test.lua")):
        t0 = time.time()
        r = subprocess.run(
            ["nvim", "--headless", "--clean", "-l", str(path)], capture_output=True, text=True, cwd=ROOT, timeout=120
        )
        results.append((f"nvim/{path.stem}", r.returncode == 0, (r.stdout + r.stderr).strip(), time.time() - t0))
    return results


def report(name, ok, out, secs=None):
    tag = f"{GREEN}PASS{RESET}" if ok else f"{RED}FAIL{RESET}"
    t = f" {DIM}({secs:.1f}s){RESET}" if secs is not None else ""
    print(f"{tag} {name}{t}")
    if not ok and out:
        lines = out.splitlines()
        print("\n".join("     " + l for l in lines[-40:]))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("filters", nargs="*")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--no-nvim", action="store_true")
    ap.add_argument("--write-docs", action="store_true")
    args = ap.parse_args()

    match = lambda tid: not args.filters or any(f in tid for f in args.filters)
    ids = [t for t in lib_ids() if match(t)]
    pyids = [t for t in py_ids() if match("py/" + t)]
    failed = 0

    print(f"== lint ({len(ids)} templates)")
    errors = lint(ids, match)
    pending = [t for t in ids if parse_header(LIB / f"{t}.cpp").get("Pending")]
    if pending:
        print(f"{DIM}pending migration ({len(pending)}): {', '.join(pending)}{RESET}")
    for e in errors:
        print(f"{RED}FAIL{RESET} {e}")
    failed += len(errors)

    with cf.ThreadPoolExecutor(args.j) as pool:
        print(f"== standalone compile")
        for tid, (ok, out) in zip(ids, pool.map(standalone, ids)):
            if not ok:
                report(f"standalone {tid}", ok, out)
                failed += 1
        print(f"== tests")
        tids = sorted([t for t in test_ids() if match(t)] + [t for t in edu_ids() if match(t)])
        for tid, (ok, out, secs) in zip(tids, pool.map(build_and_run, tids)):
            report(tid, ok, out, secs)
            failed += not ok
        print(f"== examples")
        eids = [t for t in example_ids() if match(t)]
        for tid, (ok, out, secs) in zip(eids, pool.map(run_example, eids)):
            report(f"example {tid}", ok, out, secs)
            failed += not ok

        ptids = [t for t in py_test_ids() if match("py/" + t)]
        if pyids or ptids:
            print(f"== python ({len(pyids)} templates)")
            for e in py_lint(pyids, match):
                print(f"{RED}FAIL{RESET} {e}")
                failed += 1
            for tid, (ok, out) in zip(pyids, pool.map(py_standalone, pyids)):
                if not ok:
                    report(f"standalone py/{tid}", ok, out)
                    failed += 1
            for tid, (ok, out, secs) in zip(ptids, pool.map(py_test, ptids)):
                report(f"py/{tid}", ok, out, secs)
                failed += not ok
            peids = [t for t in py_example_ids() if match("py/" + t)]
            for tid, (ok, out, secs) in zip(peids, pool.map(py_example, peids)):
                report(f"example py/{tid}", ok, out, secs)
                failed += not ok

    print("== docs")
    changed, extra = docs(ids, pyids, args.write_docs, not args.filters)
    rel = lambda p: p.relative_to(ROOT)
    if args.write_docs:
        for p in changed:
            print(f"wrote {rel(p)}")
        for p in extra:
            print(f"removed {rel(p)}")
    else:
        for p in changed:
            print(f"{RED}FAIL{RESET} {rel(p)} is stale (run tests/run.py --write-docs)")
        for p in extra:
            print(f"{RED}FAIL{RESET} {rel(p)} has no template (run tests/run.py --write-docs)")
        failed += len(changed) + len(extra)

    if not args.no_nvim and (not args.filters or "nvim" in args.filters):
        print("== nvim")
        for name, ok, out, secs in nvim_tests():
            report(name, ok, out, secs)
            failed += not ok

    print()
    if failed:
        print(f"{RED}{failed} failure(s){RESET}")
        sys.exit(1)
    print(f"{GREEN}all passed{RESET}")


if __name__ == "__main__":
    main()

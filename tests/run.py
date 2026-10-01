#!/usr/bin/env python3
"""Test runner for the C++ template library (lib/) and the Neovim helpers.

Stages:
  1. lint        every lib/<area>/<name>.cpp has a valid header, a test (tests/<area>/<name>.cpp)
                 and an example (examples/<area>/<name>.cpp)
  2. standalone  every template compiles alone (bits/stdc++.h + using namespace std + its Requires)
  3. tests       every tests/<area>/<name>.cpp compiles (-Werror, sanitizers, debug STL) and passes
  4. examples    every example compiles the same way and prints its `Output:` for its `Input:`
  5. docs        tasks/Library/ (generated from templates + examples) is up to date
  6. nvim        tests/nvim/*_test.lua pass

Usage: tests/run.py [FILTER ...] [-j N] [--no-nvim] [--write-docs]
FILTER selects templates/tests whose id ("dsa/fenwick-tree") contains it; "nvim" selects the nvim tests.
--write-docs regenerates tasks/Library/ instead of checking it.
"""

import argparse
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
FORBIDDEN = [
    (re.compile(r"^\s*#\s*include\b"), "#include (templates must be self-contained)"),
    (re.compile(r"^\s*#\s*define\b"), "#define (no macros in templates)"),
    (re.compile(r"^\s*#\s*pragma\b"), "#pragma"),
    (re.compile(r"^\s*using\s+namespace\s+std\b"), "using namespace std (already in the solution file)"),
    (re.compile(r"\bassert\s*\("), "assert (GCC 16's bits/stdc++.h no longer includes <cassert>)"),
]

GREEN, RED, DIM, RESET = ("\033[32m", "\033[31m", "\033[2m", "\033[0m") if sys.stdout.isatty() else ("",) * 4


def lib_ids():
    return sorted(str(p.relative_to(LIB).with_suffix("")) for p in LIB.rglob("*.cpp"))


def test_ids():
    return sorted(
        str(p.relative_to(TESTS).with_suffix(""))
        for p in TESTS.rglob("*.cpp")
        if p.relative_to(TESTS).parts[0] not in (".build", "nvim", "include")
    )


def example_ids():
    return sorted(str(p.relative_to(EXAMPLES).with_suffix("")) for p in EXAMPLES.rglob("*.cpp"))


def parse_header(path):
    """Header = leading `// Key: value` comment block. Indented `//   ...` lines continue the previous key."""
    meta, key = {}, None
    for line in path.read_text().splitlines():
        if not line.startswith("//"):
            break
        body = line[2:]
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
        if meta.get("Pending"):  # migration in progress: example/presets not required yet
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
    src = src or TESTS / f"{tid}.cpp"
    h = hashlib.sha256(" ".join([CXX, *TEST_FLAGS]).encode())
    for p in sorted({src.resolve(), *included_files(src)}):
        h.update(str(p).encode() + p.read_bytes())
    exe = BUILD / src.relative_to(ROOT).with_suffix("") / h.hexdigest()[:16]
    t0 = time.time()
    if not exe.exists():
        if exe.parent.exists():
            shutil.rmtree(exe.parent)
        exe.parent.mkdir(parents=True)
        r = subprocess.run([CXX, *TEST_FLAGS, str(src), "-o", str(exe)], capture_output=True, text=True)
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

def code_lines(path):
    """Source without the leading header comment block."""
    lines = path.read_text().splitlines()
    i = 0
    while i < len(lines) and lines[i].startswith("//"):
        i += 1
    while i < len(lines) and not lines[i].strip():
        i += 1
    return "\n".join(lines[i:]).rstrip()


def page_name(tid):
    return parse_header(LIB / f"{tid}.cpp")["Title"]


def gen_page(tid):
    meta = parse_header(LIB / f"{tid}.cpp")
    area = tid.split("/")[0]
    out = ["---", f"tags: [library, {area}]", "generated: true", "---", f"# {meta['Title']}", ""]
    out += ["> [!info] Generated by `tests/run.py --write-docs` from", f"> `lib/{tid}.cpp` and `examples/{tid}.cpp`. Edit those, not this page.", ""]
    out += [meta["Description"], "", f"Insert with `<leader>rl` → `{tid}`."]
    if requires(tid):
        out += ["Requires: " + ", ".join(f"[[{page_name(d)}]]" for d in requires(tid)) + " (inserted automatically)."]
    out += ["", "## Usage", "", "```cpp", meta["Usage"], "```"]
    if meta.get("Presets"):
        out += ["", "## Presets", "", "```cpp", meta["Presets"], "```"]
    out += ["", "## Complexity", "", meta["Complexity"]]
    ex = EXAMPLES / f"{tid}.cpp"
    if ex.exists():
        em = parse_header(ex)
        out += ["", "## Example", "", em.get("Problem", ""), "", "```cpp", code_lines(ex), "```"]
        if em.get("Input"):
            out += ["", "Input:", "", "```", em["Input"], "```"]
        out += ["", "Output:", "", "```", em.get("Output", ""), "```"]
    verify = [v for v in meta.get("Verify", "").splitlines() if v.strip()]
    if verify:
        out += ["", "## Verify", ""] + [f"- {v.strip()}" for v in verify]
    return "\n".join(out) + "\n"


def gen_index():
    out = ["---", "tags: [library, index]", "generated: true", "---", "# Library", ""]
    out += ["Generated by `tests/run.py --write-docs`. One page per template in `lib/`; see [[Template Library]].", ""]
    by_area = {}
    for tid in lib_ids():
        by_area.setdefault(tid.split("/")[0], []).append(tid)
    for area, tids in by_area.items():
        out += [f"## {area}", "", "| Template | Description |", "| -------- | ----------- |"]
        for tid in tids:
            meta = parse_header(LIB / f"{tid}.cpp")
            out.append(f"| [[{meta['Title']}]] | {meta['Description'].replace('|', '\\|')} |")
        out.append("")
    return "\n".join(out)


def docs(ids, write, full):
    """Returns the stale/extra pages (or the written ones when write=True)."""
    wanted = {DOCS / tid.split("/")[0] / f"{page_name(tid)}.md": gen_page(tid) for tid in ids}
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
        tids = [t for t in test_ids() if match(t)]
        for tid, (ok, out, secs) in zip(tids, pool.map(build_and_run, tids)):
            report(tid, ok, out, secs)
            failed += not ok
        print(f"== examples")
        eids = [t for t in example_ids() if match(t)]
        for tid, (ok, out, secs) in zip(eids, pool.map(run_example, eids)):
            report(f"example {tid}", ok, out, secs)
            failed += not ok

    print("== docs")
    changed, extra = docs(ids, args.write_docs, not args.filters)
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

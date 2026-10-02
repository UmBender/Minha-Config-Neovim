# End-to-end checks of the Python file templates (templates/py/*.py), run in a temp directory.
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from libtest import check, check_eq

T = Path(__file__).resolve().parents[3] / "templates" / "py"
py = sys.executable


def run(args, cwd, stdin=""):
    return subprocess.run(args, cwd=cwd, input=stdin, capture_output=True, text=True, timeout=60)


def configure(text, **cmds):
    """Set the stress template's `NAME = [...]` lines."""
    for name, value in cmds.items():
        text, n = re.subn(rf"^{name} = .*$", f"{name} = {value!r}", text, count=1, flags=re.M)
        check_eq(n, 1, f"stress.py has no {name} line")
    return text


with tempfile.TemporaryDirectory() as d:
    d = Path(d)

    # sol.py: multi-test token input, solve() left empty prints nothing
    sol = (T / "sol.py").read_text()
    check(re.search(r"^def solve\(\):\n    $", sol, re.M), "sol.py: empty solve() body to write in")
    (d / "sol.py").write_text(sol.replace("def solve():\n    \n", "def solve():\n    n = ni()\n    print(sum(nl(n)))\n"))
    r = run([py, "sol.py"], d, "2\n3\n1 2 3\n2 10\n20\n")
    check_eq((r.returncode, r.stdout, r.stderr), (0, "6\n30\n", ""))

    # gen.py: deterministic per seed, different across seeds
    gen = (T / "gen.py").read_text()
    check(re.search(r"^def gen\(\):$", gen, re.M), "gen.py has gen()")
    (d / "gen.py").write_text(gen)
    outs = [run([py, "gen.py", str(s)], d).stdout for s in (1, 1, 2)]
    check(outs[0] == outs[1] and outs[0] != outs[2] and outs[0].strip(), outs)
    check_eq(run([py, "gen.py"], d).returncode, 0)

    # euler.py runs and prints the answer of solve()
    eu = (T / "euler.py").read_text()
    check("{{N}}" in eu, "euler.py has a {{N}} placeholder for the problem number")
    (d / "euler.py").write_text(eu.replace("{{N}}", "1"))
    r = run([py, "euler.py"], d)
    check_eq((r.returncode, r.stdout.strip()), (0, "0"))

    # stress.py: generator of "n + list", a correct and a wrong solution
    (d / "gen.py").write_text(
        "import random, sys\nrandom.seed(int(sys.argv[1]))\nn = random.randint(1, 5)\n"
        "print(1)\nprint(n)\nprint(*[random.randint(-3, 3) for _ in range(n)])\n"
    )
    (d / "brute.py").write_text(sol.replace("def solve():\n    \n", "def solve():\n    n = ni()\n    print(sum(nl(n)))\n"))
    (d / "good.py").write_text("import sys\nt = sys.stdin.read().split()\nprint(' ' + str(sum(map(int, t[2:]))) + '  ')\n")
    (d / "bad.py").write_text("import sys\nt = sys.stdin.read().split()\nprint(abs(sum(map(int, t[2:]))))\n")
    (d / "crash.py").write_text("raise SystemExit(3)\n")
    stress = (T / "stress.py").read_text()

    def stress_with(solution, n, compile_cmd=[]):
        (d / "stress.py").write_text(
            configure(stress, GEN=[py, "gen.py"], SOL=[py, solution], BRUTE=[py, "brute.py"], COMPILE=compile_cmd)
        )
        return run([py, "stress.py", str(n)], d)

    r = stress_with("good.py", 60)
    check_eq(r.returncode, 0, r.stdout + r.stderr)
    check("60" in r.stdout and "OK" in r.stdout, r.stdout)  # whitespace differences are fine

    r = stress_with("bad.py", 500)
    check_eq(r.returncode, 1, r.stdout + r.stderr)
    m = re.search(r"seed (\d+)", r.stdout)
    check(m, r.stdout)
    failing = (d / "stress_input.txt").read_text()
    check_eq(failing, run([py, "gen.py", m.group(1)], d).stdout)  # the failing input is kept
    check(failing in r.stdout, "the input is shown")

    r = stress_with("crash.py", 5)
    check_eq(r.returncode, 1)
    check("exit code 3" in r.stdout, r.stdout)

    r = stress_with("good.py", 3, compile_cmd=[py, "-c", "raise SystemExit(1)"])
    check_eq(r.returncode, 1)
    check("compil" in r.stdout.lower(), r.stdout)

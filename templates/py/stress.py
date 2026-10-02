# Stress test: python3 stress.py [N] [FIRST_SEED]
# Runs GEN with seeds FIRST_SEED.. (N tests), feeds each input to SOL and BRUTE, and stops at the first
# difference (or crash / timeout), printing the input and both outputs. The input is kept in stress_input.txt.
import subprocess
import sys
import time

COMPILE = ["g++", "-std=c++20", "-O2", "-Wall", "-DLOCAL", "sol.cpp", "-o", "sol"]  # run once; [] to skip
GEN = ["python3", "gen.py"]  # the seed is appended
SOL = ["./sol"]
BRUTE = ["python3", "brute.py"]
TIMEOUT = 5  # seconds per run
SHOW = 2000  # characters of input/output shown on failure


def accept(inp, out, ans):
    """Is SOL's `out` right, given BRUTE's `ans`? Token-wise by default; edit for floats / many answers."""
    return out.split() == ans.split()


def run(cmd, inp=None):
    """(stdout, error message or None)"""
    try:
        r = subprocess.run(cmd, input=inp, capture_output=True, text=True, timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        return "", f"timeout after {TIMEOUT}s"
    if r.returncode != 0:
        return r.stdout, f"exit code {r.returncode}\n{r.stderr[-SHOW:]}"
    return r.stdout, None


def cut(s):
    return s if len(s) <= SHOW else s[:SHOW] + f"\n... ({len(s)} chars)"


def fail(seed, inp, *parts):
    with open("stress_input.txt", "w") as f:
        f.write(inp)
    print(f"\nFAILED on seed {seed} (input saved to stress_input.txt)")
    print("--- input")
    print(cut(inp))
    for title, text in parts:
        print(f"--- {title}")
        print(cut(text))
    sys.exit(1)


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
    first = int(sys.argv[2]) if len(sys.argv) > 2 else 1
    if COMPILE:
        r = subprocess.run(COMPILE, capture_output=True, text=True)
        if r.returncode != 0:
            print("compilation failed:\n" + r.stderr)
            sys.exit(1)
    start = time.time()
    for seed in range(first, first + n):
        inp, err = run(GEN + [str(seed)])
        if err:
            fail(seed, inp, ("generator", err))
        out, err = run(SOL, inp)
        if err:
            fail(seed, inp, ("SOL failed", err), ("SOL output", out))
        ans, err = run(BRUTE, inp)
        if err:
            fail(seed, inp, ("BRUTE failed", err), ("BRUTE output", ans))
        if not accept(inp, out, ans):
            fail(seed, inp, ("SOL output", out), ("BRUTE output", ans))
        print(f"\r{seed - first + 1}/{n} OK ({time.time() - start:.1f}s)", end="", flush=True)
    print(f"\r{n}/{n} OK, all passed ({time.time() - start:.1f}s)")


if __name__ == "__main__":
    main()

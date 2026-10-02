"""Shared helpers for Python library tests. Each test is one script:
    from libtest import include, check, check_eq, rnd
    include("gen/tree")          # execs pylib/gen/tree.py (and its Requires) into this module
    check_eq(got, want)
Run through tests/run.py, which puts this directory on PYTHONPATH.
"""

import random
import re
import sys
from pathlib import Path

PYLIB = Path(__file__).resolve().parents[3] / "pylib"

rnd = random.Random(0xC0FFEE)  # tests' own generator; templates use the global `random`
random.seed(12345)


def requires(tid):
    meta = {}
    for line in (PYLIB / f"{tid}.py").read_text().splitlines():
        m = re.match(r"^# (\w+):\s?(.*)$", line)
        if not line.startswith("#"):
            break
        if m:
            meta[m.group(1)] = m.group(2)
    return [r for r in re.split(r"[,\s]+", meta.get("Requires", "")) if r]


def resolve(tid, seen=None, order=None):
    seen = set() if seen is None else seen
    order = [] if order is None else order
    if tid not in seen:
        seen.add(tid)
        for dep in requires(tid):
            resolve(dep, seen, order)
        order.append(tid)
    return order


def include(tid):
    """Run the template (dependencies first) in the caller's globals, as if it was pasted there."""
    g = sys._getframe(1).f_globals
    for dep in resolve(tid):
        path = PYLIB / f"{dep}.py"
        exec(compile(path.read_text(), str(path), "exec"), g)


def check(cond, msg=""):
    if not cond:
        raise AssertionError(msg or "check failed")


def check_eq(got, want, msg=""):
    if got != want:
        raise AssertionError(f"{msg}\n  got:  {got!r}\n  want: {want!r}")

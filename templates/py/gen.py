# Random test generator: python3 gen.py SEED > input.txt (same seed, same test)
import random
import sys


def gen():
    t = 1
    print(t)
    for _ in range(t):
        n = random.randint(1, 10)
        print(n)
        print(*[random.randint(1, 10) for _ in range(n)])


if __name__ == "__main__":
    random.seed(int(sys.argv[1]) if len(sys.argv) > 1 else 0)
    gen()

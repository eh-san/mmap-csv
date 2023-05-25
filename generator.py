#!/usr/bin/python3

import os
import sys
import multiprocessing
from random import Random

dirname = "example"


def generate_input(i: int) -> None:
    filename = os.path.join(dirname, f"input{i}.csv")
    with open(filename, 'w') as csvfile:
        csvfile.write("id,duration\n")
        rng = Random(i)
        for _ in range(0, 100_000_000):
            id = 10_000_000+rng.randint(0, 9_999_999)
            duration = rng.randint(1, 10000)
            csvfile.write(f"{id},{duration}\n")


def generate_id() -> None:
    filename = os.path.join(dirname, 'ids.csv')
    with open(filename, 'w') as csvfile:
        csvfile.write("id\n")
        ids = set()
        rng = Random(99)
        while (len(ids) != 5_000_000):
            ids.add(10_000_000+rng.randint(0, 9_999_999))

        for id in ids:
            csvfile.write(f"{id}\n")


def main(argc, argv):
    if argc == 1:
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        pool = multiprocessing.Pool()
        pool.map(generate_input, range(0, 10))

        generate_id()


if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))

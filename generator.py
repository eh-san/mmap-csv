#!/usr/bin/python3

import csv
from random import Random
import multiprocessing


def generate_input(i: int) -> None:
    with open(f'input{i}.csv', 'w') as csvfile:
        csvfile.write("id,duration\n")
        rng = Random(i)
        for _ in range(0, 100_000_000):
            id = 10_000_000+rng.randint(0, 9_999_999)
            duration = rng.randint(1, 10000)
            csvfile.write(f"{id},{duration}\n")


pool = multiprocessing.Pool()
pool.map(generate_input, range(0, 10))

with open('ids.csv', 'w') as csvfile:
    csvfile.write("id\n")
    ids = set()
    rng = Random(99)
    while (len(ids) != 5_000_000):
        ids.add(10_000_000+rng.randint(0, 9_999_999))

    for id in ids:
        csvfile.write(f"{id}\n")

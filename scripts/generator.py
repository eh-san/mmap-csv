#!/usr/bin/env python3

import os
import sys
import multiprocessing
from random import Random
from dataclasses import dataclass
import argparse

SUCCESS = 0
BAD_ARGS = 1


@dataclass
class InputCSV:
    count: int = 10
    size: int = 100_000_000


@dataclass
class IDCSV:
    size: int = 5_000_000


def generate_input(count: int, size: int) -> None:
    with open(f"input{count}.csv", 'w') as csvfile:
        csvfile.write("id,duration\n")
        rng = Random(count)
        for _ in range(0, 100_000_000):
            id = 10_000_000+rng.randint(0, 9_999_999)
            duration = rng.randint(1, 10000)
            csvfile.write(f"{id},{duration}\n")


def generate_id(size: int) -> None:
    with open('ids.csv', 'w') as csvfile:
        csvfile.write("id\n")
        ids = set()
        rng = Random(99)
        while (len(ids) != 5_000_000):
            ids.add(10_000_000+rng.randint(0, 9_999_999))

        for id in ids:
            csvfile.write(f"{id}\n")


def main(argc, argv):
    if argc == 1:
        print('errror')
        return 1


    parser = argparse.ArgumentParser(description='CommandInput information')
    parser.add_argument('--count', dest='input_count', type=int,
                        help='Number of the input files')
    parser.add_argument('--input-size', dest='input_size', type=int,
                        help='Size of the input files')
    parser.add_argument('--id-size', dest='id_size', type=int,
                        help='Size of the id file')

    args = parser.parse_args()
    idcsv = IDCSV()
    inputcsv = InputCSV()
    
    if id_size:= args.id_size:
        idcsv.size = id_size
    
    if input_size := args.input_size:
        inputcsv.size = input_size

    if input_count := args.input_count:
        inputcsv.count = input_count

    pool = multiprocessing.Pool()
    pool.map(generate_input, inputcsv.count)

    generate_id()

if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))

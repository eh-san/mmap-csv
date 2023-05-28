#!/usr/bin/env python3

import os
import sys
from multiprocessing import Pool, freeze_support
from functools import partial
from random import Random
import argparse

SUCCESS = 0
ERORR = 1

def generate_input(i: int, size: int, dirname: str) -> None:
    filename = os.path.join(dirname, f"input{i}.csv")
    with open(filename, 'w') as csvfile:
        csvfile.write("id,duration\n")
        rng = Random(i)
        for _ in range(0, size):
            id = 10_000_000+rng.randint(0, 9_999_999)
            duration = rng.randint(1, 10000)
            csvfile.write(f"{id},{duration}\n")


def generate_id(size: int, dirname: str) -> None:
    filename = os.path.join(dirname, 'ids.csv')
    with open(filename, 'w') as csvfile:
        csvfile.write("id\n")
        ids = set()
        rng = Random(99)
        while (len(ids) != size):
            ids.add(10_000_000+rng.randint(0, 9_999_999))

        for id in ids:
            csvfile.write(f"{id}\n")


def main():
    parser = argparse.ArgumentParser(description='Generate CSV Input Files')
    parser.add_argument('-c', '--count', dest='count', type=int,
                        help='number of the input files')
    parser.add_argument('-p', '--path', dest='path', type=str,
                        help='path of the input files')
    parser.add_argument('--large', help='large input files',
                        action="store_true")
    parser.add_argument('--small', help='small input files',
                        action="store_true")
    args = parser.parse_args()
    count = 10 if args.count is None else args.count

    inputsize = 10_000_000
    idsize = 500_000
    if args.large:
        inputsize = inputsize * 10
        idsize = idsize * 10
    elif args.small:
        inputsize = int(inputsize / 10)
        idsize = int(idsize / 10)

    dirname = args.path if args.path else ''
    if path := dirname:
        if not os.path.exists(path):
            os.makedirs(path)
        elif not os.path.isdir(path):
            print(f"{path} must be a directory!")
            return ERORR
        
    print('generating...')

    with Pool(count) as pool:
        pool.map(partial(generate_input, size=inputsize, dirname=dirname), range(1, count + 1))

    generate_id(idsize, dirname)

    print('done.')

    return SUCCESS


if __name__ == "__main__":
    freeze_support()
    sys.exit(main())

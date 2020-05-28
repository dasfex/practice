#! /usr/bin/python3

import sys

for line in sys.stdin:
    if list(line.split('\t'))[0] != '\n':
        print(list(line.split('\t'))[0])

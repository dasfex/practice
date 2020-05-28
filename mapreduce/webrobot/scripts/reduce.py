#! /usr/bin/python3

import sys

is_visited = False
link = ''
for line in sys.stdin:
    args = list(line.split('\t'))
    if len(args) == 2 and args[1][0] == '+':
        is_visited = True
    link = args[0]
if is_visited:
    print(link, '\t', '+')
else:
    print(link, '\t')

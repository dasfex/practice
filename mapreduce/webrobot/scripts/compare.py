#! /usr/bin/python3

import sys

last_key = sys.argv[1]
key = sys.argv[2]

if key == last_key:
    raise SystemExit(1)

if abs(len(key) - len(last_key)) != 1:
    raise SystemExit(0)

if len(key) > len(last_key):
    key, last_key = last_key, key

if last_key[-1] == '/':
    last_key = last_key[:-1]
if key == last_key:
    raise SystemExit(1)
raise SystemExit(0)


#! /usr/bin/env python3

import numpy as np


def f(x):
    return np.exp(x)


def simpson_integration(a, b, n):
    n *= 2
    step = (b - a) / n
    res = f(a) + f(b)
    for i in range(1, n):
        new_x = a + step * i
        mult = 4 if i % 2 == 1 else 2
        res += f(new_x) * mult
    res *= step / 3
    return res


def main():
    print(simpson_integration(128, 524, 100000))

main()

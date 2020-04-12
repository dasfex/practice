#! /usr/bin/env python3
import sympy as sp
import numpy as np
from datetime import datetime as dt


def F(x):
    return np.exp(-x * x)


def getEquallySpacedPoints(l, r, count):
    X, Y = [], []
    sh = (r - l) / (count - 1)
    for i in np.arange(l, r + 0.0000001, sh):
        X.append(i)
        Y.append(F(i))
    return [X, Y, sh]


def getChebyshevPoints(l, r, count):
    X, Y = [], []
    for k in np.arange(1, count + 1):
        ch = np.cos((2 * k - 1) / (2 * count) * np.pi)
        x = (r + l) / 2 + (r - l) / 2 * ch
        X.append(x)
        Y.append(F(x))
    return [X, Y]


def Lagrange(Lx, Ly):
    X = sp.symbols('x')
    y = 0
    for k in range(len(Lx)):
        t = 1
        for j in range(len(Lx)):
            if j != k:
                t = t * ((X - Lx[j]) / (Lx[k] - Lx[j]))
        y += t * Ly[k]
    return y


def lagrange(l, r):
    for count in [6, 12, 18]:
        xy = getEquallySpacedPoints(l, r, count)
        start_time = dt.now()
        pol = Lagrange(xy[0], xy[1])
        end_time = dt.now()
        print('Lagrange interpolation with {} '
              'points and {} knots ended in {} seconds.'
              .format('equallyspaced', count, end_time - start_time))
    for count in [6, 12, 18]:
        xy = getChebyshevPoints(l, r, count)
        start_time = dt.now()
        pol = Lagrange(xy[0], xy[1])
        end_time = dt.now()
        print('Lagrange interpolation with {} '
              'points and {} knots ended in {} seconds.'
              .format('Chebyshev', count, end_time - start_time))


def main():
    l, r = -5, 5
    lagrange(l, r)


main()

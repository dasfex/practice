#! /usr/bin/env python3
import sympy as sp
import numpy as np
from datetime import datetime as dt


def F(x):
    return np.exp(-x * x)


def dF(x):
    return -2 * x * np.exp(-x * x)


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


def newtonPolynomial(X, Y):
    x = sp.symbols('x')
    polynom = 0
    for i in range(len(X)):
        res = Y[0][i]
        for j in range(i):
            res *= (x - X[j])
        polynom += res
    polynom = sp.simplify(polynom)
    return polynom


def hermit_interpolation(args):
    X = []
    for i in args:
        X.append(i)
        X.append(i)
    Y = np.zeros(shape=(len(X), len(X)))
    for i in range(len(X)):
        Y[i][i] = F(X[i])
    for sz in range(2, len(X) + 1):
        for i in range(len(X) - sz + 1):
            j = i + sz - 1
            if X[i] == X[j]:
                Y[i][j] = dF(X[i])
            else:
                Y[i][j] = (Y[i][j - 1] - Y[i + 1][j]) / (X[i] - X[j])
    return newtonPolynomial(X, Y)


def hermit(l, r):
    for count in [6, 12, 18]:
        x = getEquallySpacedPoints(l, r, count)[0]
        start = dt.now()
        pol = hermit_interpolation(x)
        end = dt.now()
        print('Hermit interpolation with {} '
              'points and {} knots ended in {} seconds.'
              .format('equallyspaced', count, end - start))
    for count in [6, 12, 18]:
        x = getChebyshevPoints(l, r, count)[0]
        start = dt.now()
        pol = hermit_interpolation(x)
        end = dt.now()
        print('Hermit interpolation with {} '
              'points and {} knots ended in {} seconds.'
              .format('equallyspaced', count, end - start))


def main():
    l, r = -5, 5
    hermit(l, r)


main()

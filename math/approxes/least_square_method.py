#! /usr/bin/env python3
import sympy as sp
import numpy as np
from datetime import datetime as dt


def F(x):
    return np.exp(-x * x)


def least_square_method(X, Y, n):
    count = len(X)
    H = np.zeros(shape=(2 * n + 1, count))
    G = np.zeros(shape=(n + 1, n + 1))
    for i in range(count):
        H[0][i] = 1
    for i in range(1, 2 * n + 1):
        for j in range(count):
            H[i][j] = X[j]
            for k in range(1, i):
                H[i][j] *= X[j]
    for i in range(n + 1):
        for j in range(i, n + 1):
            for k in range(count):
                G[i][j] += H[i + j][k]
    phi = np.zeros(shape=(n + 1))
    for i in range(n + 1):
        for j in range(count):
            phi[i] += Y[j] * H[i][j]
    M = [[] for i in range(n + 1)]
    for i in range(n + 1):
        for j in range(n + 1):
            if G[i][j] == 0:
                M[i].append(G[j][i])
            else:
                M[i].append(G[i][j])
    coefs = np.linalg.solve(M, phi)
    x = sp.symbols('x')
    pol = 0
    for i in reversed(coefs):
        pol *= x
        pol += i
    pol = sp.expand(pol)
    return pol


def least_square(l, r, count):
    for n in [1, 2, 4, 6]:
        X = np.random.uniform(l, r, (count))
        Y = [F(x) for x in X]
        start = dt.now()
        pol = least_square_method(X, Y, n)
        end = dt.now()
        print('Least square of power {} is builded in {} seconds.'
              .format(n, end - start))


def main():
    l, r = -5, 5
    least_square(l, r, 100)


main()

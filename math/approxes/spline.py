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


def solve_tm(m, b):
    x = np.zeros(shape=(len(b)))
    for i in range(len(m) - 1):
        if np.abs(m[i][0]) < np.abs(m[i + 1][0]):
            m[i], m[i + 1] = m[i + 1], m[i]
            b[i], b[i + 1] = b[i + 1], b[i]
        mult = m[i + 1][0] / m[i][0]
        b[i + 1] -= b[i] * mult
        for j in range(len(m[0]) - 1, -1, -1):
            m[i + 1][j] -= m[i][j] * mult
        m[i + 1].pop(0)
        m[i + 1].append(0)
    x[len(b) - 1] = b[len(b) - 1] / m[len(m) - 1][0]
    x[len(b) - 2] = (b[len(b) - 2] - m[len(m) - 2][1] *
                     x[len(b) - 1]) / m[len(m) - 2][0]
    ind = len(b) - 1
    for i in range(len(m) - 3, -1, -1):
        sm = b[i]
        for j in range(len(m[0]) - 1, 0, -1):
            sm -= m[i][j] * x[ind]
            ind -= 1
        ind += 1
        x[i] = sm / m[i][0]
    return x


def spline(X, sh, count):
    Y = np.zeros(shape=(count, count))
    for i in range(count):
        Y[i][i] = F(X[i])
    for sz in range(2, 4):
        for i in range(count - sz + 1):
            j = i + sz - 1
            Y[i][j] = (Y[i][j - 1] - Y[i + 1][j]) / (X[i] - X[j])
    m = [[] for i in range(count - 2)]
    for i in range(count - 2):
        # points are equally spaced
        m[i].append(0.5)
        m[i].append(2)
        m[i].append(0.5)
    m[0].pop(0)
    m[0].append(0)
    m[len(m) - 1].pop()
    m[len(m) - 1].append(0)
    b = [6 * Y[i][i + 2] for i in range(count - 2)]
    x = []
    if count > 3:
        x = solve_tm(m, b)
    else:
        x.append(3 * Y[0][2])
    gammas = np.zeros(shape=(len(b) + 2))
    for i in range(len(b)):
        gammas[i + 1] = x[i]
    alphas = [0 for i in range(count)]
    betas = [0 for i in range(count)]
    deltas = [0 for i in range(count)]
    vals = [0 for i in range(count)]
    for i in range(count):
        vals[i] = F(X[i])
    for i in range(1, count):
        alphas[i] = vals[i]
        betas[i] = (vals[i] - vals[i - 1]) / sh + \
                   (2 * gammas[i] + gammas[i - 1]) * sh / 6
        deltas[i] = (gammas[i] - gammas[i - 1]) / sh
    ret_spl = []
    for i in range(1, count):
        x = sp.symbols('x')
        lit_spl, g, pol = alphas[i], betas[i], x - X[i]
        g *= pol
        lit_spl += g

        g = gammas[i] / 2
        pol *= x - X[i]
        g *= pol
        lit_spl += g

        g = deltas[i] / 6
        g *= pol * (x - X[i])
        lit_spl += g

        ret_spl.append(sp.simplify(lit_spl))

    return ret_spl


def splines(l, r):
    for count in [6, 12, 18]:
        info = getEquallySpacedPoints(l, r, count)
        start = dt.now()
        spl = spline(info[0], info[2], count)
        end = dt.now()
        print('Spline with {} knots builded in {} seconds.'
              .format(count, end - start))


def main():
    l, r = -5, 5
    splines(l, r)

main()

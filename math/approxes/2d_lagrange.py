#! /usr/bin/env python3
import sympy as sp
import numpy as np
from datetime import datetime as dt


# approx surface with Lagrange in 2d
# g(x, y) = (x + y)(cos(x) + cos(y))


def f():
    x, y = sp.symbols('x y')
    return (x + y) * (sp.cos(x) + sp.cos(y))


def get_equally_spaced_points(l, r, count):
    sh = (r - l) / (count - 1)
    ret = [x for x in np.arange(l, r + 0.00000001, sh)]
    return ret


def newton_polynomial(X, Zn):
    Y = np.zeros(shape=(len(X), len(X)))
    for i in range(len(X)):
        Y[i][i] = Zn[i]
    for sz in range(2, len(X) + 1):
        for i in range(len(X) - sz + 1):
            j = i + sz - 1
            if i != j:
                Y[i][j] = (Y[i][j - 1] - Y[i + 1][j]) / (X[i] - X[j])
    y = sp.symbols('y')
    polynom = 0
    for i in range(len(X)):
        res = Y[0][i]
        for j in range(i):
            res *= (y - X[j])
        polynom += res
    return sp.simplify(polynom)


def bi_lagrange_interpolation(X, Y):
    x, y = sp.symbols('x y')
    count = len(X)
    ret = 0
    for i in range(count):
        pol_x = 1
        fs = []
        for j in range(count):
            if i != j:
                pol_x *= (x - X[j]) / (X[i] - X[j])
            fs.append(f().subs([(x, X[i]), (y, Y[j])]))
        pol_y = newton_polynomial(Y, fs)
        bipol = pol_x * pol_y
        ret += bipol
    return ret.expand()


def bi_lagrange(lx, rx, ly, ry):
    for count in [6, 12, 18]:
        x = get_equally_spaced_points(lx, rx, count)
        y = get_equally_spaced_points(ly, ry, count)
        start = dt.now()
        pol = bi_lagrange_interpolation(x, y)
        end = dt.now()
        print('Lagrange on rectangle with {} knots'
              ' built in {} seconds.'.format(count, end - start))


def main():
    lx, rx, ly, ry = 0, 11, 0, 11
    bi_lagrange(lx, rx, ly, ry)


main()

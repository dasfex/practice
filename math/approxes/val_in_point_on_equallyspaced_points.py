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


def value_in_point(X, sh, count, point):
    nghbr_points = []
    nghbr = 0
    for i in range(count):
        if np.abs(X[i] - point) <= sh:
            nghbr_points.append(X[i])
            nghbr = i
    nghbr -= 1
    Y = np.zeros(shape=(count, count))
    for i in range(2):
        Y[i][i] = F(nghbr_points[i])
    Y[0][1] = (Y[0][0] - Y[1][1]) / (-sh)

    x = sp.symbols('x')
    current_val = newtonPolynomial(nghbr_points, Y).subs(x, point)
    new_val = current_val + 1
    offset, add, sign, ind = -1, -1, -1, 1
    while abs(current_val - new_val) > 1e-10:
        nghbr_points.append(X[nghbr + add])
        sz = len(nghbr_points) - 1
        for i in range(sz, -1, -1):
            if i == sz:
                Y[i][sz] = F(nghbr_points[sz])
            else:
                Y[i][sz] = (Y[i][sz - 1] - Y[i + 1][sz]) / \
                           (nghbr_points[i] - nghbr_points[sz])
        offset *= sign
        if offset > 0:
            ind += 1
        if 0 <= nghbr + offset * ind < count:
            add = offset * ind
        elif nghbr + offset * ind >= count:
            offset, sign = 1, 1
            add = -ind
        elif nghbr + offset * ind < 0:
            sign = 1
            add += 1
        current_val = new_val
        new_val = newtonPolynomial(nghbr_points, Y).subs(x, point)

    return new_val, len(nghbr_points)


def value_in_points(l, r, count, xs):
    info = getEquallySpacedPoints(l, r, count)
    for point in xs:
        start = dt.now()
        val, cnt = value_in_point(info[0], info[2], count, point)
        end = dt.now()
        print('Value in {} point ended in {} seconds. '
              'Value is {}. {} knots.'
              .format(point, end - start, val, cnt))


def main():
    l, r = -5, 5
    value_in_points(l, r, 100, [np.sqrt(2) / 2, np.pi / 7])


main()

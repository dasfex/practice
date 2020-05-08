#! /usr/bin/env python3
import sympy as sp
import numpy as np


# cosh(y) + 2x = 45
# x^2/5 - y^2 -10x = 500
# first approx get graphically


def f1():
    x, y = sp.symbols('x y')
    return sp.cosh(y) + 2 * x - 45


def f2():
    x, y = sp.symbols('x y')
    return x**2 / 5 - y**2 + 10 * x - 500


def f(cur_x):
    x, y = sp.symbols('x y')
    return f1().subs([(x, cur_x[0]), (y, cur_x[1])]), \
           f2().subs([(x, cur_x[0]), (y, cur_x[1])])


def newton(cur_x):
    x, y = sp.symbols('x y')
    next_x = (cur_x[0] + 1, cur_x[1] + 1)
    W = np.zeros(shape=(2, 2))
    cnt = 0
    while max(np.abs(next_x[0] - cur_x[0]), np.abs(next_x[1] - cur_x[1])) >= 1e-6 and cnt <= 1000:
        if cnt != 0:
            cur_x = next_x
        cnt += 1

        W[0][0] = sp.diff(f1(), x).subs([(x, cur_x[0]), (y, cur_x[1])])
        W[0][1] = sp.diff(f1(), y).subs([(x, cur_x[0]), (y, cur_x[1])])
        W[1][0] = sp.diff(f2(), x).subs([(x, cur_x[0]), (y, cur_x[1])])
        W[1][1] = sp.diff(f2(), y).subs([(x, cur_x[0]), (y, cur_x[1])])
        inv = np.linalg.inv(W)

        f_in_point = f(cur_x)
        next_x = cur_x - np.dot(inv, f_in_point)
        next_x[0] = next_x[0].evalf()
        next_x[1] = next_x[1].evalf()

    return next_x, cnt


def get_der(f, xy, ind=0):
    eps = 1e-9
    x, y = sp.symbols('x y')
    res = f.subs([(x, xy[0]), (y, xy[1])]).evalf()
    new = 0
    if ind == 0:
        new = f.subs([(x, xy[0] * (1 - eps)), (y, xy[1])]).evalf()
    else:
        new = f.subs([(x, xy[0]), (y, xy[1] * (1 - eps))]).evalf()
    return (res - new) / (xy[ind] * eps)


def discrete_newton(cur_x):
    next_x = (cur_x[0] + 1, cur_x[1] + 1)
    W = np.zeros(shape=(2, 2))
    cnt = 0
    while max(np.abs(next_x[0] - cur_x[0]), np.abs(next_x[1] - cur_x[1])) >= 1e-6:
        if cnt != 0:
            cur_x = next_x
        cnt += 1

        lst = [f1(), f2()]
        for i in range(2):
            for j in range(2):
                W[i][j] = get_der(lst[i], cur_x, j)

        inv = np.linalg.inv(W)

        f_in_point = f(cur_x)
        next_x = cur_x - np.dot(inv, f_in_point)
        next_x[0] = next_x[0].evalf()
        next_x[1] = next_x[1].evalf()

    return next_x, cnt


def main():
    print('Newton start:')
    for x in [(-83, 6), (-82, -6)]:
        val, cnt = newton(x)
        print(val, cnt)
        print('f(x, y) = {}'.format(f(val)[0]))
    print('Newton end.')
    print('Discrete Newton start:')
    for x in [(-83, 6), (-82, -6)]:
        val, cnt = discrete_newton(x)
        print(val, cnt)
        print('f(x, y) = {}'.format(f(val)[1]))
    print('Discrete Newton end.')


main()

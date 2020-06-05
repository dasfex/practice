import sympy as sp
import numpy as np
from sympy import Matrix, linsolve
import math


# solve integral equation
# x(t) - \lambda \int_l^r ker * x(s) ds = f
# with approx kernel with degenerate kernel

def get_chebyshev_points(l, r, count, ker):
    Y = []
    for k in np.arange(1, count + 1):
        ch = np.cos((2 * k - 1) / (2 * count) * np.pi)
        X = (r + l) / 2 + (r - l) / 2 * ch
        Y.append(X)
    return Y


def interpolate(l, r, count, ker):
    points = get_chebyshev_points(l, r, count, ker)
    S = sp.symbols('s')
    W = 1
    for point in points:
        W *= (S - point)

    alphas, betas = [], []
    for point in points:
        w = W / (S - point)
        alphas.append(ker.subs('s', point))
        betas.append(w / w.subs('s', point))

    return alphas, betas


def masterEl(a, b, xi):
    return (b - a) * (xi + 1) / 2 + a


def part(a, b, f):
    xis = [-math.sqrt(0.6), 0, math.sqrt(0.6)]
    w = [5/9, 8/9, 5/9]
    sum = 0
    for i in range(3):
        sum += w[i] * f.subs('s', masterEl(a, b, xis[i]))
    return sum


def g3(a, b, count, f):
    sh = (b - a) / count
    left, right = a, b
    res = 0
    while left < right:
        res += part(left, left + sh, f)
        left += sh
    return res


def getA(al, bt, n):
    A = [[0 for i in range(n)] for j in range(n)]
    for i in range(n):
        al[i] = al[i].subs({'x': 's'})
        for j in range(n):
            A[i][j] = g3(l, r, 10, al[j] * bt[i])
    return A


def getD(b, f):
    d = []
    for beta in bt:
        beta = beta.subs('s', 'x')
        q = beta * f
        q = q.subs('x', 's')
        d.append(g3(l, r, 10, q))
    return d


x = sp.Symbol('x')
s = sp.Symbol('s')
ker = (x * s * s - 1) * sp.cos(x * s + 1)
f = 2 * x + 3
lmbd = 3/5
l, r, count = 0., 1., 9
al, bt = interpolate(l, r, count, ker)
A = np.array(getA(al, bt, count), dtype=np.float)
d = np.array(getD(bt, f), dtype=np.float)
c = np.linalg.solve(np.eye(count) - lmbd * A, d)
u = 0
for i in range(count):
    u += al[i] * c[i]
u *= lmbd
u += f
u = sp.expand(u.subs('s', 'x'))
print(u)

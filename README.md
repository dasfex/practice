# My programming practice

Hi! This is just some notes from my algo and not only practice. 
If you find some mistake please create an issue or pull request with fixes. Thanks!

## Data structures

1. [Binary heap](https://github.com/dasfex/Algorithms/blob/master/data_structures/binary_heap.h).
2. [Set with iterators](https://github.com/dasfex/ProgrammingPractice/blob/master/data_structures/set.h).
3. [Unordered map with iterators](https://github.com/dasfex/ProgrammingPractice/blob/master/data_structures/unordered_map.h).
4. [Segment tree(sum) with 2N memory use](https://github.com/dasfex/Algorithms/blob/master/data_structures/segment_tree.cpp).
5. [Segment tree with lazy propogation](https://github.com/dasfex/Algorithms/blob/master/data_structures/lazy_seg_tree.cpp).
6. [DSU(disjoint set union)](https://github.com/dasfex/Algorithms/blob/master/data_structures/dsu.cpp).
7. [DSU with backup](https://github.com/dasfex/Algorithms/blob/master/data_structures/backup_dsu.cpp).
8. [Basic Bloom filter](https://github.com/dasfex/Algorithms/blob/master/data_structures/basic_bf.h).
9. [Counting Bloom filter](https://github.com/dasfex/Algorithms/blob/master/data_structures/counting_bf.h).
10. [Explicit treap](https://github.com/dasfex/Algorithms/blob/master/data_structures/explicit_treap.cpp).
11. [Implicit treap](https://github.com/dasfex/Algorithms/blob/master/data_structures/implicit_treap.cpp).
12. [Fenwick tree](https://github.com/dasfex/Algorithms/blob/master/data_structures/fenwick_tree.cpp).
13. [Queue](https://github.com/dasfex/Algorithms/blob/master/data_structures/advance_queue.cpp) based on two stacks with template functor.
14. [Bidirectional list](https://github.com/dasfex/Algorithms/blob/master/data_structures/bidirectional_list.cpp).
15. [Deque](https://github.com/dasfex/Algorithms/blob/master/data_structures/deque.h) based on vector of blocks(without invalidation when container relocate).
16. [Cow-vector](https://github.com/dasfex/Algorithms/blob/master/data_structures/cow_vector.h)(relocate if and only if we want to change it and do not relocate if read-only).
17. [Intrusive list](https://github.com/dasfex/Algorithms/blob/master/data_structures/intrusive_list.h).

## Math algorithms

#### [Matrix algos](https://github.com/dasfex/Algorithms/tree/master/math/matrices)

1. [LU decomposition](https://github.com/dasfex/Algorithms/blob/master/math/matrices/LU.h).
2. [LDLT decomposition](https://github.com/dasfex/Algorithms/blob/master/math/matrices/LDLT.h).
3. [Gauss method to find inverse matrix](https://github.com/dasfex/Algorithms/blob/master/math/matrices/gauss.h).
4. [Matrix transposition](https://github.com/dasfex/Algorithms/blob/master/math/matrices/transposition.h).
5. [Tridiagonal matrix algorithm](https://github.com/dasfex/Algorithms/blob/master/math/matrices/TMA.h).
6. [QR algorithm to find eigenvalues of matrix and eigenvectors of symmetric matrix(Givens rotation, Householder transformation)](https://github.com/dasfex/Algorithms/blob/master/math/matrices/QR.h).

#### [Approxes](https://github.com/dasfex/Algorithms/tree/master/math/approxes)

1. [Lagrange interpolation](https://github.com/dasfex/Algorithms/blob/master/math/approxes/lagrange.py).
2. [Hermit interpolation(with Newton interpolation)](https://github.com/dasfex/Algorithms/blob/master/math/approxes/hermit.py).
3. [Spline interpolation](https://github.com/dasfex/Algorithms/blob/master/math/approxes/spline.py).
4. [Least square method](https://github.com/dasfex/Algorithms/blob/master/math/approxes/least_square_method.py).
5. [2D Lagrange](https://github.com/dasfex/Algorithms/blob/master/math/approxes/2d_lagrange.py).
6. [Value in point on 100 equallyspaced points](https://github.com/dasfex/Algorithms/blob/master/math/approxes/val_in_point_on_equallyspaced_points.py).
7. [Newton method for solving nonlinear system](https://github.com/dasfex/Algorithms/blob/master/math/approxes/newton_nonlin_syst.py).
8. [Power method to find max eigenvalue](https://github.com/dasfex/Algorithms/blob/master/math/approxes/PowerMethod.h).
9. [Simpson integration](https://github.com/dasfex/Algorithms/blob/master/math/approxes/simpson_integration.py).
10. [Count definite integral with five points method and adaptive steps](https://github.com/dasfex/Algorithms/blob/master/math/approxes/five_points_method.cpp).
11. [Count definite integral with Gauss-3 and adaptive steps](https://github.com/dasfex/Algorithms/blob/master/math/approxes/gauss_3.cpp).
12. [Solve integral equation with approximation kernel with degenerate kernel](https://github.com/dasfex/Algorithms/blob/master/math/approxes/integral_eq_solver.py).

#### [Other](https://github.com/dasfex/Algorithms/tree/master/math/other)

1. [Binary exponential](https://github.com/dasfex/Algorithms/blob/master/math/other/binary_exp.cpp).
2. [Eiler function](https://github.com/dasfex/Algorithms/blob/master/math/other/EilerFunction.cpp).
3. [Fast finding nth fibonacci number modulo m](https://github.com/dasfex/Algorithms/blob/master/math/other/fibonacci.cpp).
4. [Fast finding nth fibonacci number modulo m on compilation time](https://github.com/dasfex/Algorithms/blob/master/math/other/compile_fibonacci.cpp).
5. [Danilevskiy algo to find characteristic polynomial and Newton algo to find real roots](https://github.com/dasfex/Algorithms/blob/master/math/other/Danilevskiy%26Newton.h).

#### [Some algos in pictures](https://github.com/dasfex/Algorithms/tree/master/math/some_algos_in_pictures)

## Graphs

1. [Dijkstra algorithm](https://github.com/dasfex/Algorithms/blob/master/graphs/Dijkstra.cpp).
2. [LCA(less common ancestor) with binary lifts](https://github.com/dasfex/Algorithms/blob/master/graphs/Lca.cpp).
3. [Find all minimum spanning trees in O(ans * nmlog(n))](https://github.com/dasfex/Algorithms/blob/master/graphs/msts.cpp).
4. [Parallel Kruskal](https://github.com/dasfex/Algorithms/blob/master/graphs/parallel_kruskal.cpp).
5. [Check two trees for isomorphism](https://github.com/dasfex/ProgrammingPractice/blob/master/graphs/trees_isomorphic.cpp)(this is probability solver).
6. [Some examples](https://github.com/dasfex/Algorithms/tree/master/graphs/exmp).

## C++

#### Algos

0. [Generate all permutations in lexicographic order](https://github.com/dasfex/Algorithms/blob/master/cpp/algos/permutations.cpp).
1. [LRU-cache](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/algos/lrucache.h).

#### Classes

1. [Vector](https://github.com/dasfex/Algorithms/blob/master/cpp/classes/vector.cpp).
2. [Unique ptr](https://github.com/dasfex/Algorithms/blob/master/cpp/classes/unique_ptr.cpp).
3. [Shared and weak ptr](https://github.com/dasfex/Algorithms/blob/master/cpp/classes/smart_ptr.h).
4. [Any](https://github.com/dasfex/Algorithms/blob/master/cpp/classes/any.h).

#### Concurrency

1. [FindIf(multithreading function to find all numbers in some range that satisfies some unary predicate)](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/concurrency/find_if.h).
2. [Reduce(multithreading function than compute result of some associative commutative binary predicate to all vector elements)](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/concurrency/reduce.h).
3. [Spinlock](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/concurrency/spinlock.h).
4. [Read-write lock](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/concurrency/rw_lock.h).
5. [Buffered channel(class that provides synchronization between threads when store some values(in buffer))](https://github.com/dasfex/Algorithms/blob/master/cpp/concurrency/buffered_channel.h).
6. [Unbuffered channel(same as buffered channel but without buffer)](https://github.com/dasfex/Algorithms/blob/master/cpp/concurrency/unbuffered_channel.h).
7. [Concurrent unordered map](https://github.com/dasfex/ProgrammingPractice/blob/master/cpp/concurrency/concurrent_unordered_map.h).

## Golang

0. [Sprintf](https://github.com/dasfex/ProgrammingPractice/blob/master/golang/format.go).
1. [LRU-cache](https://github.com/dasfex/ProgrammingPractice/blob/master/golang/lrucache.go).

## Linux programming

1. [File system tasks](https://github.com/dasfex/Algorithms/tree/master/linux_programming/file_system).
2. [Processes tasks](https://github.com/dasfex/Algorithms/tree/master/linux_programming/processes).
3. [Shell tasks](https://github.com/dasfex/Algorithms/tree/master/linux_programming/shell).
4. [Shell2 tasks](https://github.com/dasfex/Algorithms/tree/master/linux_programming/shell2).
5. [GNU cat](https://github.com/dasfex/Algorithms/blob/master/linux_programming/cat.c).
6. [Custom pipeline](https://github.com/dasfex/Algorithms/blob/master/linux_programming/pipeline.c).

## Windows programming

1. [Pipeline](https://github.com/dasfex/Algorithms/blob/master/windows_programming/pipeline.cpp).
2. [Wrapper for winapi conditional variable](https://github.com/dasfex/Algorithms/blob/master/windows_programming/conditional_variable.h).
3. [Wrapper for winapi critical section](https://github.com/dasfex/Algorithms/blob/master/windows_programming/critical_section.h).

## MapReduce

1. [Inplace(with solve for WordCount)](https://github.com/dasfex/Algorithms/tree/master/mapreduce/inplace).
2. [Web Robot](https://github.com/dasfex/Algorithms/tree/master/mapreduce/webrobot).

## Other

1. [Rational class](https://github.com/dasfex/Algorithms/blob/master/other/rational_class.cpp).
2. [External sort](https://github.com/dasfex/Algorithms/tree/master/other/external_sort).
3. [Get hex string from number(C++)](https://github.com/dasfex/Algorithms/blob/master/other/hex_string_from_number.cpp).

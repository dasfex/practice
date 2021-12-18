/*
 * Solve this task on compilation time:
 * Turtle Sara wants to get as much as possible coins.
 * At the beginning she is on the (0, 0) cell of the field.
 * She wants to get in the (n - 1, m - 1) cell.
 * Coins in (i, j) field is ((i * K + j) ^ (j * K + i)) % M for some K M.
 */

#include <iostream>
#include <vector>

constexpr uint32_t K = 228;
constexpr uint32_t M = 1'000'000'007;

template<uint32_t A, uint32_t B>
struct Max {
    constexpr static auto value = A > B ? A : B;
};

template<uint32_t I, uint32_t J>
struct Cell {
    constexpr static auto value = ((I * K + J) ^ (J * K + I)) % M;
};

template<uint32_t I, uint32_t J>
struct Dp {
    static constexpr auto value =
            Max<Dp<I - 1, J>::value, Dp<I, J - 1>::value>::value +
            Cell<I, J>::value;
};

template<uint32_t J>
struct Dp<0, J> {
    static constexpr auto value = Dp<0, J - 1>::value + Cell<0, J>::value;
};

template<uint32_t I>
struct Dp<I, 0> {
    static constexpr auto value = Dp<I - 1, 0>::value + Cell<I, 0>::value;
};

template<>
struct Dp<0, 0> {
    static constexpr auto value = 0;
};

template<uint32_t N, uint32_t M>
struct Turtle {
    static constexpr auto value = []() {
        Dp<N - 1, 0>;
        Dp<0, M - 1>;
        return Dp<N - 1, M - 1>::value;
    }();
};

template <uint32_t N, uint32_t M>
inline constexpr uint32_t Turtle_v = Turtle<N, M>::value;

int main() {
    std::cout << Turtle_v<13, 19>;

    return 0;
}


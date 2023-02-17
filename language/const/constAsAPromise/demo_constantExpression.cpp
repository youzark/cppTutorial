#include <cstddef>
void foo(size_t n) {
    int x[n];
    int y[10];
    int z[10 + 10];
}

void foo_non_const_expr(size_t n) {
    int const level = n;
    int x[level + 1];
}

void foo_must_const_expr(size_t n) {
    constexpr int level_err = n; // ERROR
    constexpr int level = 40;
    int x[level + 1]; //OK
}

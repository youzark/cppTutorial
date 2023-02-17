#include <iostream>


int func(int const& j) {
    std::cout << j << std::endl;
    return j;
}

class String {};

String operator+(String const& loper,String const& roper);  // take in both rvalue and lvalue
                                                            // return by-value to form a temporary

int main() {
    int const i = 10;
    func(i);
    func(10);
    func(i + 10);
}

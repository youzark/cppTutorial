#include <iostream>
#include <string>

void foo(std::string const& msg) {
    std::cout << msg <<std::endl;
    std::cout << "const& Version get called" << std::endl;
}

void foo(std::string && msg) {
    std::cout << msg <<std::endl;
    std::cout << "&& Version get called" << std::endl;
}

int main() {
    std::string testNormal = "std::string test1";
    std::string testMove = "std::move<std::string>";

    foo(testNormal);
    foo(testNormal + " aggregate with literal");
    foo("literal");
    foo(std::move(testMove));
}

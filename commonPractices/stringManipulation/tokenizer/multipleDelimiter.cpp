#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

int main() {
    std::string input{"10*9-(12+8)/2"};
    std::string delimiter{"+-*/()"};
    auto start = input.begin(), end = input.end();
    for( auto cur = start; cur != end;) {
        auto nextDelimiter = std::find_first_of(cur,end,delimiter.begin(),delimiter.end());
        std::string token{cur,nextDelimiter};
        if(!token.empty()) std::cout << token << std::endl;
        cur = nextDelimiter == end ? end : ++nextDelimiter;
    }
}

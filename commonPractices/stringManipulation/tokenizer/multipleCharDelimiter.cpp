#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

int main() {
    std::string input{"youzark+-+is+-+my+-+name"};
    std::string delimiter{"+-+"};
    int deliLen = delimiter.size();
    
    auto start = input.begin(), end = input.end();
    for( auto cur = start; cur != end; ) {
        auto nextDeli = std::search(cur,end,delimiter.begin(),delimiter.end());
        std::string token{cur,nextDeli};
        if(!token.empty()) std::cout << token << std::endl;
        cur = nextDeli == end ? end : nextDeli + deliLen;
    }
}

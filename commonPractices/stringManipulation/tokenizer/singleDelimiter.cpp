#include <sstream>
#include <string>
#include <iostream>

int main() {
    std::string input{"I'm youzark\nI'm trying to teach the best practices on c++ programming language\nIt's tough\n"};
    std::istringstream ss{input};
    for(std::string line; std::getline(ss,line);) {
        std::istringstream lineStream{line};
        for(std::string token; std::getline(lineStream,token,' ');) {
            std::cout << token << " | ";
        }
        std::cout <<std::endl;
    }
}

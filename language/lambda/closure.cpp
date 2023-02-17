#include <iostream>
#include <string>
#include <vector>
#include <ostream>


std::ostream& operator<<(std::ostream& stream,const std::vector<std::string>& other) {
    for(auto& line : other) {
        stream << line << std::endl;
    }
    return stream;
}

auto blockMsgger(std::vector<std::string> blockMsg) {
    return [blockMsg]() {std::cout << blockMsg;};
}

auto msgger(std::string msg) {
    auto lambda = [msg]() {std::cout << msg << std::endl;};
    return lambda;
}

int main() {
    auto lambda = msgger("Close");
    auto blkMsg = blockMsgger({"My name is youzark","I live as a vimer"});
    blkMsg();
    lambda();
}

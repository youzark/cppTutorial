#pragma once
#include <string>

class Printer {
public:
    Printer(std::string,int);
    void Print(std::string);
private:
    std::string m_name;
    int m_availablePaper;
};


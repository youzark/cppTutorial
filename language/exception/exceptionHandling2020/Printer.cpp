#include "Printer.hpp"
#include <iostream>

Printer::Printer(std::string name,int paper):
    m_name(name),
    m_availablePaper(paper) {
        std::cout << "Printer<" << m_name << "> with " << paper << " pages" << " constructed!" << std::endl;
    }
void Printer::Print(std::string txtDoc) {
    std::cout << "Printing...\n" << txtDoc <<std::endl;
}

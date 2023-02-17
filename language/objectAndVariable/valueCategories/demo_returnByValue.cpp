#include <algorithm>
#include <cstddef>
#include <string>
#include <cstring>
#include <iostream>
#include <utility>

class Str {
private:
    char* m_str;
    size_t m_size;

public:
    // default constructor
    Str(): 
        m_str(nullptr),
        m_size(0)
    {
        std::cout << "Default Constructor Called!" << std::endl;
    }
    // convert constructor
    Str(char const* rhl):
        m_size(std::strlen(rhl))
    {
        if(rhl == 0) {
            throw "Invalid Initialization from nullptr";
        }
        m_str = new char[m_size+1];
        std::strcpy(m_str,rhl);
        std::cout << "Convert Constructor Called!" << std::endl;
    }
    // destructor
    ~Str() {
        delete[] m_str;
        std::cout << "Destructor called" << std::endl;
    }

    // Copy constructor
    Str(Str const& rhs):
        m_size(rhs.m_size)
    {
        m_str = new char[m_size + 1];
        std::strcpy(m_str,rhs.m_str);
        std::cout << "Copy Constructor Called!" << std::endl;

    }
    // Move Constructor
    Str(Str && rhs):
        m_str(std::exchange(rhs.m_str,nullptr)),
        m_size(std::exchange(rhs.m_size,0)) 
    {
        std::cout << "Move Constructor Called!" << std::endl;
    }

    // Move Assignment
    Str& operator=(Str &&rhs) {
        m_str = std::exchange(rhs.m_str,nullptr);
        m_size = std::exchange(rhs.m_size,0);
        std::cout << "Move Assignment Called!" << std::endl;
        return *this;
    }
    
    // Copy Assignment
    Str& operator=(Str const& rhs) {
        Str copy{rhs};
        m_str = std::exchange(copy.m_str,nullptr);
        m_size = std::exchange(copy.m_size,0);
        std::cout << "Copy Assignment Called!" << std::endl;
        return *this;
    }

    void print() const {
        std::cout << m_str << std::endl;
    }

    Str operator+(Str const&rhs) const {
        return std::strcat(this->m_str,rhs.m_str);
    }

    void inspectString() const {
        std::cout << "Valid: " << (m_str[m_size+1] == 0 && std::strlen(m_str) == m_size) << std::endl;
        std::cout << "Content: " << m_str << std::endl;
    }
};

/* Str operator+(Str const& lfh, Str const& rhs) { */
/*     return std::strcat(lfh.m_str,rhs.m_str); */
/* } */


int main() {
    Str a{"My Name is: "};
    Str b{"youzark"};

    std::cout << "****************************************" << std::endl;
    std::cout << "Test use return by-value to Initialize: " << std::endl;
    std::cout << "****************************************" << std::endl;
    Str c{a+b};
    std::cout << "\nOnly convert constructor gets called , which means actually no copy happened" << std::endl;
    std::cout << "########################################\n\n" << std::endl;

    std::cout << "****************************************" << std::endl;
    std::cout << "Test use return by-value to Assign: " << std::endl;
    std::cout << "****************************************" << std::endl;
    Str d{};
    d = a+b;
    std::cout << "a+b returns A temporary object which is by default rvalue" << std::endl;
    std::cout << "the returns temporary get moved into d and destructed immediately after evaluation" << std::endl;
    std::cout << "########################################\n\n" << std::endl;

}



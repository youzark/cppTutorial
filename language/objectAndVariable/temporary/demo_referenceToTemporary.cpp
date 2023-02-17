#include <string>
#include <iostream>
using namespace std;

class Sandbox
{
public:
    Sandbox(const string& n) : member(n) {}
    const string& member; // when you declare a const reference as a member variable.
                          // you basically have the access to read some resources(variable) while
                          // not have the right to control that resources, so you don't know when
                          // will the reference get invalidated, that's a behaviour must be thinking 
                          // carefully.

};

void printVar(double const& i) {
    std::cout << i << std::endl;
}

int main() {

    Sandbox sandbox(string("temporary literals bind to class member const reference"));
    cout << "life time: " << sandbox.member << endl; // dangling reference here , undefined behaviour


    printVar(10);   //conversion happens first (int)10:prvalue -> (double)10:xvalue ,
                    //this will create a temporary (double)10:xvalue.
                    //(double)10:xvalue will be bind to double const& i(lvalue) : they both are glvalue(xvalue|lvalue)
                    //after expression evaluation finished, temporary is removed.
    std::cout << "printVar(10); finished" << std::endl; // (double)10:xvalue not exist here



    int const& i = 10;
    string const& test{"temporary literals bind to local const reference"};
    std::cout << "life time: " <<  i << std::endl;
    std::cout << "life time: " <<  test << std::endl;
}


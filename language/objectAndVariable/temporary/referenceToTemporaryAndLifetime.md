## problem set

```cpp
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

int main()
{
    Sandbox sandbox(string("four"));
    cout << "The answer is: " << sandbox.member << endl;
    return 0;
}
```

Decription:
1. Sandbox sandbox(string("four")); will create a temporary for string("four") 
2. sandbox stores an reference to that temporary
3. what's the lifetime of that temporary?

## temporary object creation
> Temporary objects are created when a prvalue is materialized so that it can be used as a glvalue
>
> -- <cite>[cpp reference](https://en.cppreference.com/w/cpp/language/lifetime) 

## temporary object lifetime

### general rule
All temporary objects are destroyed as the last step in evaluating the full-expression that (lexically) contains the point where they were created, and if multiple temporary objects were created, they are destroyed in the order opposite to the order of creation. This is true even if that evaluation ends in throwing an exception.

```cpp
void printVar(double const& i) {
    std::cout << i << std::endl;
}

int main() {
    printVar(10);   //conversion happens first (int)10:prvalue -> (double)10:xvalue ,
                    //this will create a temporary (double)10:xvalue.
                    //(double)10:xvalue will be bind to double const& i(lvalue) : they both are glvalue(xvalue|lvalue)
                    //after expression evaluation finished, temporary is removed.
    std::cout << "printVar(10); finished" << std::endl; // (double)10:xvalue not exist here

}
```
### special case

1. The lifetime of a temporary object may be extended by binding to a const lvalue reference(locally not a class member).
```cpp
int main() {
    int const& i = 10;
    string const& test{"temporary literals bind to local const reference"};
    std::cout << "life time: " <<  i << std::endl;      //temporary i will servive here
    std::cout << "life time: " <<  test << std::endl;   //temporary test will servive here
}
```

2. The lifetime of a temporary object created when evaluating the default arguments of a default or copy constructor used to initialize or copy an element of an array ends before the next element of the array begins initialization.

Ellustrated with introduction example.

3. The lifetime of temporaries created in the range-for loop initializer that would otherwise be destroyed at the end of the initializer are extended to the end of the loop body, except for the temporaries that are function parameters.

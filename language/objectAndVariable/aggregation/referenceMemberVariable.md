## Problem description
**Deprecated: A Better Solution is Found** 

The relationship described in the following code is when An object want to have 
access to another object while not own it.  
The correct way to get things done is to use share_ptr and weak_ptr rather than 
use reference.
```cpp
#include <iostream>
using namespace std;

class A
{
public:
   A(int& thing) : m_thing(thing) {}
   void printit() { cout << m_thing << endl; }

protected:
   const int& m_thing; //usually would be more complex object
};


int main(int argc, char* argv[])
{
   int myint = 5;
   A myA(myint);
   myA.printit();
   return 0;
}
```

idiom:
    construct class object by reference

### Is there a name to describe this idiom?

In UML it is called aggregation. It differs from composition in that the member object is not owned by the referring class. In C++ you can implement aggregation in two different ways, through references or pointers.

### common usage
 The main reason for aggregation is that the contained object is not owned by the containing object and thus their lifetimes are not bound. In particular the referenced object lifetime must outlive the referring one. It might have been created much earlier and might live beyond the end of the lifetime of the container. Besides that, the state of the referenced object is not controlled by the class, but can change externally. If the reference is not const, then the class can change the state of an object that lives outside of it.

### pitfalls
The most common pitfall is that the lifetime of the object holding the reference must never exceed the lifetime of the referenced object. If the enclosing object uses the reference after the referenced object was destroyed, you will have undefined behavior. 

### composition v.s. aggregation
composition:
    The member object is not owned by the referring class
aggregation:
    


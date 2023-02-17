# RAII <==> Resource Allocation is Intialization

## Classed that manage resources

### Resource

- Allocated Memory
- POSIX file handles
- C FILE handles
- Mutex locks (pthread_mutex_lock/pthread_mutex_unlock)
- C++ threads (spawn/join)

Some Resources are intrinsically Unique(lock); while others are duplicable.
![classManageHeapAllocation](/home/youzark/projects/cppTutorial/language/resources/raii/img/classManageHeapAllocation.png)

### A naive implementation of vector

```cpp
class NaiveVector {
    int *ptr_;
    size_t size_;
public:
    // constructor
    NaiveVector():
        ptr_(nullptr),
        size_(0)
    {
    }

    // Copy constructor
    NaiveVector(NaiveVector const& rhs) {
        ptr_ = new int[rhs.size_];
        size_ = rhs.size_;
        std::copy(rhs.ptr_,rhs.ptr_ + rhs.size_,ptr_);
    }

    // Destructor
    ~NaiveVector() {
        delete [] ptr_;
    }

    // move constructor
    NaiveVector(NaiveVector&& rhs) noexcept {
        ptr_ = std::exchange(rhs.ptr_,nullptr); // assign to nullptr so that the original destructor won't touch the 
                                                // resources
        size_ = std::exchange(rhs.size_,0);
    }

    void swap(NaiveVector& rhs) noexcept {
        using std::swap;
        swap(ptr_,rhs.ptr_);
        swap(size_,rhs.size_);
    }

    friend void swap(NaiveVector& a, NaiveVector& b) { // support std::swap(NaiveVector&,NaiveVector&) overload
        a.swap(b);
    }

    // copy assignment 
    NaiveVector& operator=(NaiveVector const& rhs) {
        NaiveVector copy{rhs};
        copy.swap(*this);
        return *this;
    }

    NaiveVector& operator=(NaiveVector && rhs) {
        NaiveVector copy{std::move(rhs)};
        copy.swap(*this);
        return *this;
    }

    void push_back(int newvalue) {
        int *newptr = new int[size_ + 1];
        std::copy(ptr_,ptr_ + size_,newptr);
        delete [] ptr_;
        ptr_ = newptr;
        ptr_[size_++] = newvalue;
    }

};
```

### The Rule of Three(manually manage resources)
If your class directly manage some kind of resources ,three member functions must be implement manually:

1. destructor
2. copy constructor
3. copy assignment operator (use Copy-and-swap idiom)

### Why copy-and-swap
detailed talk see ~/projects/cppTutorial/language/exception/exceptionHandling.md

1. no self-assignment problem.
2. strongly exception safety guarantee. 
3. for templated or recursive data structure, (shared_ptr), like picture below
![deleteOnRecursiveDataStructure](/home/youzark/projects/cppTutorial/language/resources/raii/img/deleteOnRecursiveDataStructure.png)

In the Picture above , assign rhs to v without using copy-and-swap idiom cause visit deallocated memory

### RAII and exception safety

> "Resource Acquisition Is Initialization"
>
> The slogan is about initialization , but its meaning is really about cleanup

When exception happens, functions on the call stack cannot finish their instructions.
Thus the statement responsible for release resources might not get called.
But if we manage resources by classes, destructors will get called automatically as stack unwinding.
So at least those resources won't leak.

### delete special functions
To ban user copy assign or construct , we delete those functions
```cpp
struct RAIIPtr {
    int *ptr_;
    RAIIPtr(int *p) : ptr_(p) {};
    ~RAIIPtr() { delete [] ptr_; }

    RAIIPtr(RAIIPtr const&) = delete;
    RAIIPtr& operator=(RAIIPtr const&) = delete;
};
```

### default explicitly
```cpp
struct RAIIPtr {
    int *ptr_;
    RAIIPtr(int *p) : ptr_(p) {};
    ~RAIIPtr() { delete [] ptr_; }

    RAIIPtr(RAIIPtr const&) = default;
    RAIIPtr& operator=(RAIIPtr const&) = default;
};
```
Set special functions default is mainly claims that "default one works fine"

### The Rule of Zero(No manual management of resources)

If your class does not directly manage any resources(uses library components) , don't write special member functions on your own:
- Let Compiler generate defaulted destructor
- Let Compiler generate copy constructor
- Let Compiler generate copy assignment operator
- maybe write your own swap

### rvalue references

rvalue must appear on the right-hand side.

#### rvalue & lvalue binding
as a general rule:
- rvalue do not bind to lvalues
- lvalue do not bind to rvalues
- Special Case: const lvalue reference bind to rvalue

```cpp
int i = 42;

void lf(int &);
lf(i); // OK
lf(42); // ERROR

void rf(int &&);
rf(i); // ERROR
rf(42); // OK

void clf(int const&);
clf(i);  // OK
clf(42); // OK
```

#### when combined with overloading
```cpp
#include <iostream>
#include <string>
void foo(std::string const& msg) {
    std::cout << msg <<std::endl;
    std::cout << "const& Version get called" << std::endl;
}

void foo(std::string && msg) {
    std::cout << msg <<std::endl;
    std::cout << "&& Version get called" << std::endl;
}

int main() {
    std::string testNormal = "std::string test1";
    std::string testMove = "std::move<std::string>";

    foo(testNormal);
    foo(testNormal + " aggregate with literal");
    foo("literal");
    foo(std::move(testMove));
}
```

### Move constructor

### The Rule of Five

If your class directly manage some kind of resources ,three member functions must be implement manually:

1. destructor
2. copy constructor
3. move constructor
4. copy assignment operator (use Copy-and-swap idiom)
5. move assignment operator

### duplication of move assignment and copy assignment

```cpp
class NaiveVector {
    int *ptr_;
    size_t size_;
public:
    // constructor
    NaiveVector(): ptr_(nullptr), size_(0) { }

    // Copy constructor
    NaiveVector(NaiveVector const& rhs) ;

    // Destructor
    ~NaiveVector() { delete [] ptr_; }

    // move constructor
    NaiveVector(NaiveVector&& rhs) noexcept ;

    // copy assignment 
    NaiveVector& operator=(NaiveVector const& rhs) {
        NaiveVector copy{rhs};
        copy.swap(*this);
        return *this;
    }

    // move assignment
    NaiveVector& operator=(NaiveVector && rhs) {
        NaiveVector copy{std::move(rhs)};
        copy.swap(*this);
        return *this;
    }

    void push_back(int newvalue) ;

};
```

### By-value assignment operator?
```cpp
NaiveVector& NaiveVector::operator=(NaiveVector copy) {
    copy.swap(*this);
    return *this;
}
```
Since one copy must be made for copy-and-swap ,why not use by-value ,which pass in exactly a copy.
Thougth not common in real-life coding anyway

### The Rule of Four and a half
1. destructor
2. copy constructor
3. move constructor
4. by-value assignment operator
5. nonmember swap

### Closer-to-Rule-of-Zero vector
![byUniquePtr](/home/youzark/projects/cppTutorial/language/resources/raii/img/byUniquePtr.png)


### Examples of Resource Management



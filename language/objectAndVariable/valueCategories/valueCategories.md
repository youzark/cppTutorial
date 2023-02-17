# Value Category

Behaviour as:
1. right hand side
2. left hand side
3. parameter(operands)
4. return value
5. result of subexpression

## Evolution of category

```plantuml
:Early C:
rvalue and lvalue;
:Early C++:
Introduce const reference and classes;
:Modern C++:
More value categories;
```

### lvalues & rvalue

* what's lvalue:
    1. only lvalue can be left operand (can appear on either side of the an expression)
    2. any expression referring to an object(object refer to a piece of memory)

* Why distinct rvalue from lvalue:
    1. rvalue not necessarily occupy memory
    2. immediate
        
True for Builtin types , not true for classes!!!


####  Literals

* numeric literals
* character literals ( usually do occupy storage )

#### lvalue-to-rvalue conversion

#### What about Result Value
Results are temporary , sometimes in register, rvalue(no memory guaranteed)

#### Unary * yield a lvalue

### Summary

Compiler insist the freedom not assigning memory to rvalue, although sometimes it will.
You have to pretent no memory assigned.

Compiler promise assigning memory to lvalue, although sometime it won't
You have to pretent memory exist.


### Rvalues of Class Type
To access class member variables , we use offset from a base address in c++,
which means evenif rvalue of a class must be stored in memory.

### Non-Modifiable Lvalues

const lvalue can't be assigned


## Reference Types

reference acts like a alias or ( a const pointer that dereference itself everytime get called )

The real reason Reference is introduced is to overload operators(functions):
- like for ++class_type , we cannot pass parameter by value or by pointer ,where by reference is a must

![operatorOverloadByValue](/home/youzark/projects/cppTutorial/language/reference/valueCategories/img/operatorOverloadByValue.png)
![operatorOverloadingByPointer](/home/youzark/projects/cppTutorial/language/reference/valueCategories/img/operatorOverloadingByPointer.png)


### Reference to Const Parameters
R f(T const& t);

"reference to const" parameter accepts both const or non-const

"reference to non-const" parameter accepts only non-const value

### Reference and temporaries

#### The general rule

```cpp
int *pi = &3; // ERROR
int &ri = 3;  // ERROR

int i;
double *pd = &i; // ERROR
double &rd = i;  // ERROR
```


#### reference to const T bind to rvalue Non-T
When conversion happens, a temporary created, which is where the reference bound to.
```cpp
double const &rd = 3;
```
This mechanism is designed for :
```cpp
void f(long double const& ld);
f(1);
```

### Two kinds of Rvalues

1. Pure rvalues (prvalue) : don't occupy data storage

2. Expiring values (xvalue) : do occupy data storage

#### possible scene:

1. rvalue(except for string) of builtin type is born prvalue.
2. rvalue of class object will occupy memory
3. temporary materialization conversion will turn prvalue to xvalue


### Mimicking builtin operators

for instance builtin "+":
1. operands may be lvalues or rvalues
2. return value must be rvalues

See demo_returnByValue.cpp for detail
```cpp
class Str {
private:
    char* m_str;
    size_t m_size;

public:
    Str operator+(Str const&rhs) const {            // return Str by value mimic a + b is a temporary and cannot be modified
        return std::strcat(this->m_str,rhs.m_str);
    }
};

```

### Rvalue Reference

```cpp
int &&ri = 10; // allowed

double &&f(int &&ri);   // must notice that what's return is still a reference , if the temporary it refers to get 
                        // deallocated ,you will get a dangling reference

int const &&rci = 20; // allowed , but not so useful

int n = 10;
int &&ri = n;           // Error: 
int const &&rj = n;     // Error:
```

### Rvalue References as Lvalues

if we pass a temporary(rvalue) into a functions that takes rvalue parameters ,how will the parameter interpreted inside 
the functions?

```cpp
string &string::operator=(string &&other) {
    string temp(other); // calls string(string const &)
}
```

Inside the function:

parameter that declared as rvalue get bind to a parameter name, and the rule here is:
> if it has a name , it's an lvalue

So for the callee , the parameter declared as a rvalue actually treated a lvalue internally, which survive the whole function call.

However, for the caller ,the passed-in parameter is indeed a temporary which usually survive the function call statement.

### Lvalues as Xvalues
```cpp
template <typename T>
void swap(T &a, T &b) {
    T temp(a);  // copy construction
    a = b;      // copy assignment
    b = temp;   // copy assignment
}
```

It's safe to say temp is goint to expire whatever , we should inform compiler that it's safe to use move assignment
```cpp
template <typename T>
void swap(T &a, T &b) {
    T temp(a);  // copy construction
    a = b;      // copy assignment
    b = std::move(temp);   // move assignment
}
```

## Value Category
![valueCategory](/home/youzark/projects/cppTutorial/language/objectAndVariable/valueCategories/img/valueCategory.png)



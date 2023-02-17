# Const As A Promise
[source vid](https://www.youtube.com/watch?v=NZtr93iL3R0&list=PLHTh1InhhwT4CTnVjJqnAKeMfGzOWjsRa&index=12) 

## Uses for const

1. To Define symbolic constants?
    - Yes , but...
    - constexpr is better
2. To define immutable data
    - yes, but...
    - constexpr is better
3. To prevent modifying a potentially modifiable operand?
    - yes
    - when passing arguments and returning value by pointer or reference

### Using const is Good Hygiene

1. turn run-time bugs into compile-time errors
2. make easier to use correctly interfaces

Compared with constexpr:

constexpr turn run-time computations into compile time computations.

## What Const Means functionally

* Mandated Initialization
you must initialize it at definition(not necessarily in declaration)

* Constant Expressions:
    - Can have operators and multiple operands
    - must be evaluated at compile time

```cpp
void foo(size_t n) {
    int x[n]; // Error
    int y[17]; // OK
    int y[17 + 89]; // OK
}
```

* const Object don't always result in const expressions
```cpp
void foo_non_const_expr(size_t n) {
    int const level = n; // Although const obj , but initialized at run-time
    int x[level + 1];
}
```

* constexpr Object Mandated to form constant expressions
```cpp
void foo_must_const_expr(size_t n) {
    constexpr int level_err = n; // ERROR
    constexpr int level = 40;
    int x[level + 1]; //OK
}
```
object defined as constexpr must be evaluated as compile-time , which makes 
constexpr a better keyword to define a symbolic constants.

* cv-qualifier
    - const
    - volatile
    - both  

constexpr is not a cv-qualifier

## Structure of Declaration (objects and functions): 

![StructureOfDeclaration](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/StructureOfDeclaration.png)
declaration:
- declaration specifiers
    - type specifier: builtin type or user defined type
    - non-type specifier: deal with scope ,memory durition and things like that(extern,static,inline,typedef)
- declarator
    - operators
    - declarator-id

### Precedence rules in declaration
![declaratorOperatorPrecedence](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/declaratorOperatorPrecedence.png)

Notice the Precedence Here is kinda counter intuitive:
```cpp
int *x[N];
```
x is a array of pointers
```cpp
int (*x)[N];
```
x is a pointer to arrays

### parentheses in Declarators

- As fucntion call operator:
    - follow declarator-id
    - same Precedence as subscription[]

- As grouping operator
    - enclose declarator-id
    - highest Precedence
![parentheses](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/parentheses.png)

### Type vs. Non-type Specifiers
- type specifiers modify other type specifier
- Non-type specifiers apply directly to the declarator-id
![Type_vs_Non_type](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/Type_vs_Non_type.png)

## How does const work syntacticaly?
const can be either declaration specifiers or part of declarator !!!

### const is a Type-Specifier
- order of the declaration specifiers doesn't matter to the compiler

```cpp
unsigned long ul;
long unsigned lu; // Same as ul

const unsigned long cul;
long unsigned const luc; //Same as cul
unsigned const long ucl; //Same as cul
```

- const is Type-Specifier
![how_to_interpret_const](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/how_to_interpret_const.png)

### const in declarators!

const and volatile are the only symbols that can appear as declaration specifiers or in declarators

![constIndeclarator](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/constIndeclarator.png)

- const appearing to the immediate right of * in a declarator turns the pointer into a "const pointer"
```cpp
widget *const cpw; // const pointer to ..
widget *const *cpw; // pointer to const pointer to ..
widget **const cpw; // const pointer to pointer to ..
```
Here you have to remember the counter_intuitive interpretation of operator Precedence in declarators.

### What about constexpr?
- constexpr is also a declaration specifier (non-type)
![constexpr_interpretation](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/constexpr_interpretation.png)
![constexpr_interpretation2](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/constexpr_interpretation2.png)

### Address-Of & preserve constness
```cpp
int i;
int const ci = 43;

&i;     // pointer to [non-const] int
&ci;    // pointer to const int
```

### Array-to-Pointer Conversion preserve constness

### interpretation of const by example

```cpp
const T *p; // [non-const] pointer to const T
T const *p; // same

p = &x; // OK: pointer itself is modifiable
*p = x; // ERROR: object p points to is non-modifiable
```

```cpp
T *const p; // const pointer to [non-const] T

p = &x; // ERROR
*p = x; // OK
```

```cpp
T const *const p; // const pointer to const T
const T *const p; // const pointer to const T

p = &x; // ERROR
*p = x; // ERROR
```

## Conversions Involving const
```cpp
T *pointer;
T const *pointerToConst;

void wantPointer(T *);
void wantPointerToConst(T const*);

wantPointer(pointer) // OK for sure!
wantPointerToConst(pointerToConst) // OK for sure!

wantPointer(pointerToConst) // ERROR: but why?
wantPointerToConst(pointer) // OK : but why?
```

### Think Const as a Promise

![intuition](/home/youzark/projects/cppTutorial/language/const/constAsAPromise/img/intuition.png)

```cpp
T a;
T const *qc = &a;   // promise not modify a through qc
T *q = &a;          // still can change a through q
```

```cpp
T const* pc;        // make promise don't change object point to.
void wp(T *q);      // doesn't make promise not change
wp(pc);             // pc doesn't trust wp in not alter the object it point to ,so ERROR
```

### Qualification Conversion
you can convert a pointer to a more cv-qualified pointer

```cpp
T *p;
T const *pc;
T volatile *pv;

pc = p;     //OK: adds const
p = pc;     //Error: loses const
pv = p;     //OK: add volatile
p = pv;     //Error: lose volatile
pv = pc;    //Error
pc = pv;    //Error
```

### Reference Initialization

- Reference initialization behaves similarly to qualification conversions for pointers.
    - Reference initialization can increase the constness of the referenced object
    - Reference initialization can't decrease the constness of the referenced object

### Iterators

- Standard iterators are generalized pointer
- Standard container define:
    - iterator:  point to an element in a container
    - const_iterator:   point to element in a const-qualified container

conversion between iterator also only happen when constness doesn't decrease.


## Top-Level CV-Qualifier as Parameter is meanless

### Level of types:
* pointer to char
    1. pointer to
    2. char
* array of pointer to int
    1. array of
    2. pointer to
    3. int
* string
    1. string


### Top level cv-qualifier:

```cpp
T *const p;             // Top-Level CV-Qualifier: const
T const *volatile q;    // Top-Level CV-Qualifier: const

T x;                    // No top-level
T const volatile *p;    // No top-level
```

Top-Level const in Parameter Declaration is pretty useless (should be discourage)

## Const and Class Design
### const member function
if we define a const object of a class type , then this object cannot call member function unless that function 
is declared as const(after all ,member function have access to all member variables that may alter the object)
```cpp
class string {
public:
    size_t size();
};

string const t;
for(int i = 0, i < t.size(), i++) {}    //ERROR , t is const , but t.size() does not promise to be const 
```

```cpp
class string {
public:
    size_t size() const;    // Equivalent to  size_t size(string const *this);
};

string const t;
for(int i = 0, i < t.size(), i++) {}    //OK
```

## logical const vs. bitwise const

```cpp
class string {
public:
    char &operator[](size_t i ) const {
        return text[i];
    }
}
R foo(string const &s) {
    for( size_t i = 0; i < s.size(); i++) {
        if(s[i] == something) {             // OK: you are always welcome to visit a const member function
            s[i] = otherthing;              // OK: But really a logic error here, the member function is mark as const
                                            // it will not modify s inside the member function it self
                                            // but it do return a non-const reference ,which opens the door to break
                                            // const promise
        }
    }
}
```

### Solution: Overloading const
if we brutely modify return type of operator[], we are not allowed to use it correctly even in non-const case.

```cpp
class string {
public:
    char const &operator[](size_t i ) const {
        return text[i];
    }
}
R foo(string &s) {
    for( size_t i = 0; i < s.size(); i++) {
        if(s[i] == something) {             // OK: you are always welcome to visit a const member function
            s[i] = otherthing;              // ERROR: Cause the return value is a reference to const
        }
    }
}
```

Actually the ideally solution should be something like this:
```cpp
class string {
public:
    char const &operator[](size_t i) const {
        return text[i];
    }
    char &operator[](size_t i) {
        return text[i];
    }
}
R foo(string &s) {
    for( size_t i = 0; i < s.size(); i++) {
        if(s[i] == something) {             // OK: you are always welcome to visit a const member function
            s[i] = otherthing;              // OK: the non-const version is called
        }
    }
}
R foo(string const &s) {
    for( size_t i = 0; i < s.size(); i++) {
        if(s[i] == something) {             // OK: you are always welcome to visit a const member function
            s[i] = otherthing;              // ERROR: the const version is called, and the return is reference to const
        }
    }
}

```













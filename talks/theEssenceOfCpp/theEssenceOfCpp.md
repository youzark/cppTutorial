# The Essence of C++

<!--toc:start-->
- [The Essence of C++](#the-essence-of-c)
  - [1. what's C++](#1-whats-c)
    - [1.1 design purpose (  )](#11-design-purpose)
    - [1.2 what's cpp](#12-whats-cpp)
    - [1.3 whole picture](#13-whole-picture)
  - [2. Resource Management](#2-resource-management)
    - [2.1 what's resource management](#21-whats-resource-management)
    - [2.2 in C++](#22-in-c)
    - [2.3 pointer misuse](#23-pointer-misuse)
      - [2.3.1 naked pointer](#231-naked-pointer)
      - [2.3.2 shared_ptr trick and deficiency (like in Java)](#232-sharedptr-trick-and-deficiency-like-in-java)
      - [2.3.3 unique_ptr](#233-uniqueptr)
      - [2.3.4 managed by Handle(RAII)](#234-managed-by-handleraii)
      - [2.3.5 Error Handling and Resources in C++](#235-error-handling-and-resources-in-c)
    - [2.4 Why on earch we still need pointers(references & iterators)](#24-why-on-earch-we-still-need-pointersreferences-iterators)
      - [2.4.1 what we don't do](#241-what-we-dont-do)
      - [2.4.2 what we can do](#242-what-we-can-do)
    - [2.5 how do we return?](#25-how-do-we-return)
      - [2.5.1 WRONG_WAY_1: return pointer to a new'd object](#251-wrongway1-return-pointer-to-a-newd-object)
      - [2.5.2 WRONG_WAY_2: return reference to a new'd object](#252-wrongway2-return-reference-to-a-newd-object)
      - [2.5.3 WRONG_WAY_3 pass a target object?](#253-wrongway3-pass-a-target-object)
      - [2.5.4 what's the problem](#254-whats-the-problem)
      - [2.5.5 return by handle(with move semantics)](#255-return-by-handlewith-move-semantics)
    - [2.6 Last word on GC and resource management](#26-last-word-on-gc-and-resource-management)
  - [2. Some word on OOP](#2-some-word-on-oop)
    - [2.1 Class hierarchies](#21-class-hierarchies)
    - [2.2 some words on OOP](#22-some-words-on-oop)
  - [3. Generic Programming : Template](#3-generic-programming-template)
    - [3.1 Design goals](#31-design-goals)
    - [3.2 Template nowadays](#32-template-nowadays)
    - [3.3 Algorithms](#33-algorithms)
      - [3.3.1 half-open](#331-half-open)
      - [3.3.2 parameterize criteria,actions,algorithms(Function objects and lambda)](#332-parameterize-criteriaactionsalgorithmsfunction-objects-and-lambda)
      - [3.3.3 Clear implementation of Template function(Algorithm) objects](#333-clear-implementation-of-template-functionalgorithm-objects)
      - [3.3.4 Container Algorithm (best practice)](#334-container-algorithm-best-practice)
    - [3.4 the third goal of template: concept](#34-the-third-goal-of-template-concept)
      - [3.4.1 some notation](#341-some-notation)
      - [3.4.2 Better error message](#342-better-error-message)
      - [3.4.3 Generic Programming is just Programming](#343-generic-programming-is-just-programming)
      - [3.4.4 Paradigms : OOP ,Generic Programming, Traditional Programming](#344-paradigms-oop-generic-programming-traditional-programming)
  - [4. challenges](#4-challenges)
<!--toc:end-->


## 1. what's C++

### 1.1 design purpose (  )

1. type safety

more strict type checking without hinder performance

2. resource safety

memory ,file handles, communication connection, locks ...

3. performance

run as fast as possible , close to hardware

4. predictability

for real time system

5. Teachability

"has to be novices friendly"

6. Readability

### 1.2 what's cpp

direct map to hardware(like C) + Zero-overhead abstraction(idea called OOP now)

abstraction: build structures that represent real life objects on you own

### 1.3 whole picture

C++ is best tool for building efficient and elegant abstractions for resource constrained applications
(like OS , Game Engines , Embeded system)

## 2. Resource Management

### 2.1 what's resource management

* require & release
    * explicitly and implicitly
    * shouldn't be manual (no leaks)
* owner required (who is responsible for management of resources)
    * by handle(represent something meaningful)

### 2.2 in C++

Container

thread,lock_guard,istream,fstream,unique_ptr ...

constructor, destructor( hierachically )

### 2.3 pointer misuse

#### 2.3.1 naked pointer
```cpp
void f(int n,int x) {
    Gadget* p = new Gadget{n}; // in java style (but no implicit GC supported)
    // if function returns,throw exception before delete , resources leaks
    if(x < 100) throw std::runtime_error("Weired"); // leak: destructor is no called
    if(x < 200) return; //leak: destructor is no called
    delete p; //look like no garbage collector
}
```
Problems:
1. garbage collection don't release non-memory collection ,when we don't know what's in gadget, it's dangerous to use garbage collector 
2. early return will cause resources leakage(destructor not called)

#### 2.3.2 shared_ptr trick and deficiency (like in Java)
shared_ptr is simple implementation of garbage collection

```cpp
void f(int n,int x) {
    auto p = make_shared<Gadget>(n); // with GC
    if(x < 100) throw std::runtime_error{"Weird!"}; // no leakage
    if(x < 200) return;
}
```
**Problems**
some resources needs no sharing, why bother to count?

#### 2.3.3 unique_ptr
Everything shared comes with a cost
```cpp
void f(int n,int x) {
    auto p = make_unique<Gadget>(n); // automatically managed pointer, but no use_count, much cheaper
    if(x < 100) throw std::runtime_error{"Weird!"}; // no leakage
    if(x < 200) return;
}
```

**Problems**
why pointer in the first place? if the objects is onwed by myself?

#### 2.3.4 managed by Handle(RAII)
```cpp
void f(int n,int x) {
    Gadget g{n}; // Destructor will be called automatically when function exits, so no need to call destructor at all
                 // Early return will cause no trouble too
    if(x < 100) throw std::runtime_error{"Weird"};
    if(x < 200) throw return;
}
```

#### 2.3.5 Error Handling and Resources in C++
1. "Resource Acquisition is Intialization"
    1. Acquire during construction
    2. Release in destructor
2. Throw exception in case of failure
3. Never throw while holding a resource not owned by a handle
4. Leave established invariants intact when leaving a scope

### 2.4 Why on earch we still need pointers(references & iterators)

#### 2.4.1 what we don't do

* **don't** : represent ownership ,use handle instread
* **don't** : return large amount of data out of a function

#### 2.4.2 what we can do

* to reference resources from within a handle
* to represent positions carefully
* to pass large amounts of data into function


### 2.5 how do we return?

when do we encounter this problem?
- factory function
- a function that return (multiple) big objects

#### 2.5.1 WRONG_WAY_1: return pointer to a new'd object
```cpp
M* operator+(const M&,const M&);
M* pm = m1 + m2; // who is responsible for deletion of returning objects?
M* q = *pm + m3; // who is responsible for deletion of returning objects?
```

No one takes responsiblity for deletion of the new'd objects ,and the code is ugly!!!

#### 2.5.2 WRONG_WAY_2: return reference to a new'd object
```cpp
M& operator+(const M&,const M&);
M m = m1 + m2; // who does the deletion?
```
looks better , but still can cause memory leakage

#### 2.5.3 WRONG_WAY_3 pass a target object?
```cpp
void operator+(const M&,const M&,M& res);
operator+(m1,m2,m)
```
too ugly to be accepted

#### 2.5.4 what's the problem

when we try to return an object

* copying is expensive
* tricks to avoid copying are brittle
* tricks to avoid copying are not general

The answer here is to return a handle, but how? given an object gets delete out of it's scope

#### 2.5.5 return by handle(with move semantics)(in nature still return by-value)
```cpp
Matrix operator+(const Matrix& a, const Matrix& b) {
    Matrix r;  // defined in operator+ function, which means it's lifetime equals the function's
    // copy a[i][j] + b[i][j] into r[i][j]
    return r; // to make this not return by copy, we use move semantics(defined below)
}
Matrix res = a + b;
```

Move semantics that make magic happens:( actually for the reason of copy elision(RVO return value optimization) , even move is not needed if not assign but instruct directly )
```cpp
class Matrix {
    Representation rep;
    Matrix(Matrix&& a) {
        rep = a.rep;
        a.rep = {};
    }
};
```

Notice: copy and move semantics can be generated automatically under some cases.

### 2.6 Last word on GC and resource management

GC is neither general nor ideal

Apply these techniques in order:
1. Store data in Container
2. Manage all resources with resource handles
3. Use smart pointer
4. Plug in a garbage collector

one example:
```cpp
template<typename C,typename V>
vector<Value_type<C>*> find_all(C& c,V v) {
    vector<Value_type<C>*> res;
    for(auto&x:c) {
        if(x==v) res.push_back(&x);
    }
}

```

## 2. Some word on OOP

### 2.1 Class hierarchies

class hierarchies

base class and interface

protection model

no universal bass class

multiple inheritance

Minimal run-time type identification (no reflection)

### 2.2 some words on OOP

It's Great:

1. hierarchical concepts
2. run-time selection of alternatives

It comes with costs:

1. polymorphism bring ~25% of overhead ,which goes down when happens multiple times
2. not inlined good
3. memory overhead
4. Greater coupling
5. Designing good interface requires foresight

## 3. Generic Programming : Template

### 3.1 Design goals

1. Extremely general/ flexible
2. Zero-overhead
3. Well-specified interfaces(Not fully achieved yet)
    1. overloading
    2. good error message
    3. separate compilation

### 3.2 Template nowadays

1. STL container, STL Algorithms ...
2. Compile-time duck typing(if it walks like a duck ,it quacks like a duck)
3. Better support for compile time programming ( e.g. constexpr )

### 3.3 Algorithms

1. half-open [first:last)
2. parameterize with element type and container type
3. Can also parameterize which makes program more flexible:
    1. container type
    2. element type
    3. criteria, action, algorithms(flexibility and performance)
4. General function object
    1. can carry state
    2. easily inlined
    3. Lambda notation
5. nice and clean implementations
6. container algorithms

#### 3.3.1 half-open
```cpp
void f(vector<int>& v,list<string>& lst) {
    sort(v.begin(),v.end());
    auto p = find(lst.begin(),lst.end(),"Aarhus");
    if(p != lst.end()) {}
}
```

#### 3.3.2 parameterize criteria,actions,algorithms(Function objects and lambda)
```cpp
void g(vector<string>& vs) {
    auto p = find_if(vs.begin(),vs.end(),Less_than{"Griffin"});
}

```

Less_than can be defined with Function objects or lambda

Function obj:
```cpp
struct Less_than {
    string s;  // hold state
    Less_than(const string& ss) :s(ss) {}
    bool operator()(const string& v) const { return v<s; }
};
```

With Lambda:
```cpp
auto p = find_if(vs.begin(),vs.end(),
    [](const string& v) {return v<"Griffin";});
```

#### 3.3.3 Clear implementation of Template function(Algorithm) objects
```cpp
template<typename Iter, typename Predicate>
Iter find_if(Iter first, Iter last, Predicate pred) {
    while(first != last && !pred(*first)) ++first;
    return first;
}
```

#### 3.3.4 Container Algorithm (best practice)
```cpp
template<typename Seq, typename Pred>
void sort(Seq& s, Pred p) {
    std::sort(begin(s),end(s),p);
}
```

### 3.4 the third goal of template: concept

A predicate on a set of types and values

```cpp
template<typename Cont>
    requires Sortable<Cout>() // Sortable : random access sequence with comparible element
void sort(Cont& container);
```

#### 3.4.1 some notation
There are some syntactic sugar for requires syntax.
```cpp
// the following three code snippets works the same way

void sort(Sortable& c); // version1

template<Sortable __S> // version2
void sort(__S& c);

template<typename __S> //version3
    requires Sortable<__S>()
void sort(__S& c);

```
which is useful for lambda
```cpp
[](Sortable& c){sort(begin(c),end(c));}
```

#### 3.4.2 Better error message
```cpp
template<Sortable Cont>
    void sort(Cout& container);

vector<double> vec{1.2,4.2,4.6,-1.2};
list<int> lst{1,4,9,5,2,6};

sort(vec); // OK
sort(lst); // Error: list is not random access container
```

the error message:"list<int> dose not satisfy the constraint 'Sortable'"

#### 3.4.3 Generic Programming is just Programming

#### 3.4.4 Paradigms : OOP ,Generic Programming, Traditional Programming

Much of the distinction between three of them is an illusion:

The following code snippet deploies both "OOP" and "GP"

```cpp
void draw_all(Container& c)
    requires Same_type<Value_type<Container>,Shape*>() {
    for_each(c,[](Shape* p){ p->draw();})
}
```

## 4. challenges

* how to make programmer prefer modern C++ code style
* how to make C++ better with respect to compatibility with C
* how to improve and complete the techniques and models embodied in C++

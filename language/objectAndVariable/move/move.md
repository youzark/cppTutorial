# The Basics of Move Semantics

Look at value category first as prerequisite.

## Move Constructor

### What's std::move ?

cast lvalue unconditionally to rvalue.
```cpp
template< typename T >
std::remove_reference_t<T>&&
move( T && t ) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}
```

### behavior of Default move constructor

```cpp
class declaredmovable {
public:
    declaredmovable() = default;
    declaredmovable(const declaredmovable &) { std::cout << "declaredmovable: copied!"<< std::endl; }
    declaredmovable(declaredmovable &&) { std::cout << "declaredmovable: moved!"<< std::endl; }
};

class deletedmovable {
public:
    deletedmovable() = default;
    deletedmovable(const deletedmovable &) { std::cout << "declaredmovable: copied!"<<std::endl; }
    deletedmovable(deletedmovable &&) = delete;
};

class defaultmovable {
public:
    defaultmovable() = default;
    defaultmovable(const defaultmovable &) { std::cout << "defaultmovable: copied!" << std::endl; }
    defaultmovable(defaultmovable &&) = default;
};

class nodelcaremovable {
public:
    nodelcaremovable() = default;
    nodelcaremovable(const nodelcaremovable &) { std::cout << "nodelcaremovable : copied!" << std::endl; }
};

class hasnonmovable {
private:
    defaultmovable def{};
    declaredmovable dec{};
    nodelcaremovable nodec{};
    deletedmovable del{};
public:
    hasnonmovable() = default;
    /* hasnonmovable(hasnonmovable &&) = default; */
};

class onlyMovable {
private:
    defaultmovable def{};
    declaredmovable dec{};
    nodelcaremovable nedec{};
public:
    onlyMovable() = default;
    onlyMovable(onlyMovable &&) = default;
};

int main() {
    std::cout << "-----------------------------------\n";
    std::cout << "Test: Will move happen when move constructor is define : 1: to be default, 2:manually 3:not Declared\n";
    defaultmovable def{};
    defaultmovable mdef{std::move(def)};
    declaredmovable dec{};
    declaredmovable mdec{std::move(dec)};
    nodelcaremovable nodec{};
    nodelcaremovable mnodec{std::move(nodec)};

    std::cout << "-----------------------------------\n";
    std::cout << "Test:All member movable: Explicitly Declared Default | Manually Declared | No Declaration \n";
    onlyMovable o{};
    onlyMovable mo{std::move(o)};
    std::cout << "-----------------------------------\n";

    std::cout << "-----------------------------------\n";
    std::cout << "Test:Exist non-movable member: Explicitly Declared Default | Manually Declared | No Declaration | Explicitly deleted \n";
    hasnonmovable h{};
    hasnonmovable mh{std::move(h)};
    std::cout << "-----------------------------------\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Conclusion:\n";
    std::cout << "Declare move constructor(assignment) \"as default\" when posible, only then will the class be recognized as move constructable!\n";
    std::cout << "If member variable contain non-movable object, declare your own move special member function to move the movable part\n";
}
```

- General rule:
    - Keep rule of zero to make code elegent
    - If there are non-movable(move constructor = delete) member varible in 
    class , no default move constructor & assignment operator can be generated, 
    you have to write your own version by hand.
    - If you defined some object to manage raw resources with raw pointer or 
    handlers which do not respect rule of five ,write your own move constructor. 
    (or wrap the raw pointer/handler into a class that respect rule of five, 
    like with smart_ptr)

- Expected Behaviour of default move constructor/assignment:
    - noexcept
    - call member varible's move constructor in order if defined.
    - will not reset raw pointer's value(Which might cause double deletion if not taken care of)
    
- rules for default generation
![default_generate](/home/youzark/projects/cppTutorial/language/objectAndVariable/move/img/default_generate.png)

> **Core Guideline C.21:** If you define or =delete any default operation ,define or =delete them all!

I think this rule of default special member function generation is to prevent 
double deletion and resource leakage problem in classes that manage raw resources 
without noticing.

Cause default move constructor and assignment operator won't do pointer reset and 
stale resources clean-up for you. So if there are raw resources(like a piece of 
new'd heap memory or a FILE* handler), in which case a destructor and copy-constructor 
must be defined manually ,a default move constructor and operator won't be generated 
automatically.

### Manually write a move constructor
```cpp
class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};
public:
    Widget( Widget&& w ):
        i ( std::move(w.i) ),                   // std::move make no difference this line ,but keep it uniform.
        s ( std::move(w.s) ),                   // std::move make w.s movable to s
        pi ( std::exchange(w.pi , nullptr) )    // make pi point to what w.pi point to and redirect w.pi to nullptr
    { }
};
```
**Steps to Write a Move Constructor** 
1. Member-wise move
2. reset(raw pointer)

The default constructor will and only will do step one.

### Mark move constructor noexcept

It will make the performance much better if the function that takes the moved 
object make a "Strong Exception Safty Guarantee".
- if the move constructor is noexcept, it can be moved directly even if Strong Exception
Safty is Guaranteed.
- if not ,we need to do a copy first( then use swap which is noexcept ).

```cpp
class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};
public:
    Widget( Widget&& w ) noexcept:              // mark as noexcept when possible
        i ( std::move(w.i) ),                   // std::move make no difference this line ,but keep it uniform.
        s ( std::move(w.s) ),                   // std::move make w.s movable to s
        pi ( std::exchange(w.pi , nullptr) )    // make pi point to what w.pi point to and redirect w.pi to nullptr
    { }
};

Widget w{ 1,"A long long string that is not small string optimizable", nullptr};
v.push_back( std::move(w) )                     // if move constructor is not marked as noexcept ,it won't be used here
```

By the way, **The Best Practice** here:  
Since you are managing two different resources in Widget class:
* 1: string
* 2: raw array of int with a pointer pi point to.

String is a class that will do construction and destruction automatically(well managed).  
But the array of int which pi point to will not construct and destruct by itself, 
that's the reason why we need define move constructor manually.

The advice here is to manage the array of int by a class rather than by raw pointer.  
we can either wrap it in smart_prt or define a class(which respect rule of five) for it.

The general idea here is:  
when we use classes to represent some resources, it's either one raw resources 
managed manually or several resources already managed by their own classes.

### A move constructor should move and leave its source in a valid state(sometimes with default value).

the default will not set the source object to default value by the way.


## Move Assignment Operator
```cpp
class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};
public:
    Widget& operator=( Widget && rhs ) {    
        // self-move-assignment(cause by state oneself movable, it's treated as a expiring value) is different to self-copy-assignment
        delete pi;              //cause the resources is managed by raw pointer, we need to delete it whenever the one 
                                //responsible for manage it point to something else
        i = std::move(rhs.i);
        s = std::move(rhs.s);
        pi = std::exchange(rhs.pi,nullptr);     //To avoid double deletion problem.
        
        return *this;
    }
};
```

**Steps to write move assignment:** 
1. clean up
2. Member-wise move
3. reset(raw pointer)

The default move assignment operator will and only will do step 2.

### Copy-Move operator:
```cpp
    // Copy-Move assignment
    Widget& operator=( Widget w ) 
    {
        swap( w );
        return *this;
    }
    void swap( Widget& w ) {
        // use std::swap member wise.
    }
```
Strictly speaking , this implementation is a copy assignment. But it can handle the move assignment like below:
1. when the passed in parameter is a lvalue ,a copy of it will be created by 
calling copy constructor, we can just swap the copied version. that's the expected 
behaviour of copy assignment operator.

2. when the passed in parameter is a rvalue ,the move constructor will be called 
to pass the parameter to the assignment operator. Which means no actual copy is 
made, we just swap the moved parameter. that's the expected behaviour of move assignment 
operator.

## Conventions of passing parameter and return value:
> **Core Guideline C.15:** Prefer simple and conventional ways of passing information.

![parameterAndReturnValue](/home/youzark/projects/cppTutorial/language/objectAndVariable/move/img/parameterAndReturnValue.png)

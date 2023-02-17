[source video](https://www.youtube.com/watch?v=3jCOwajNch0&list=PLHTh1InhhwT4CTnVjJqnAKeMfGzOWjsRa&index=7) 

# Back to basics: Lambda from Scratch

## 1.

### 1.1 Old plain function

#### 1.1.1 Funtion Overloading

```cpp
int plus1(int x) {
    return x+1;
}

double plus1(double x) { // Function Overloading : static polymorphism
    return x+1;
}
```
To achieve function overloading , when compile, Cpp mangle parameter type with function for linker

#### 1.1.2 Template
```cpp
template<typename T>
T plus1(T x) {
    return x+1;
}
```
When compiler encounter a template, it deduce what parameter type is from function call and generate corresponding 
code , which is slightly different from function overloading ones.

#### 1.1.3 Class Member Function

```cpp
class Plus {
    int value;
public:
    Plus(int v): value(v) {};
    int plusme(int x) const {
        return x + value;
    }
};

auto plus = Plus(1);
auto x = plus.plusme(42); // how does C++ compiler decide which function to call in the class?
assert(x == 42)
```

#### 1.1.4 The Java Solution when determine which Class Function to Call

The object that lives in heap maintain a vtable of vpointers that points to different functions(class behaviours) which 
resides in static code and data section.

#### 1.1.5 The C++ approach when determine which Class Function to Call
I will resides purly in stack , it might manage allocation on the heap.

no vptr if no virtual

the compiler knows which function to call statically(maybe inline too)

#### 1.1.6 Operator Overloading

### Why Lambdas?

Lambdas provide implementation of closures mechanism for C++.

#### what is **Closures** in programming language

A closure is the combination of a function bundled together (enclosed) with references to its surrounding state (the lexical environment). In other words, a closure gives you access to an outer function's scope from an inner function.

1. closures have access to non-local variable in enclosing function. like in python
```python
def namePrinter(name):
    def printName():
        print("My name is " + name)
    printName()

namePrinter("youzark")
```
2. closures visit non-local variables by capture them , so statements within closures might still visit those variables
evenif the original copy get destroyed(if capture by value rather than by reference or pointer).
```python
def namePrinterFactory(name):
    def printName():
        print("My name is " + name)
    return printName  # closure function get return

namePrint = namePrinterFactory("youzark") # "youzark" as a parameter "name" to outer function,
                                          # parameter name has been released when namePrinterFactory returns
namePrint() # But inner function(closures) still have access to "youzark"
```

#### Closures in C++ and Lambdas
```cpp
class Plus {
    int value;
public:
    Plus(int v): value(v) {}
    int operator() (int x) const {
        return x + value;
    }
};
auto plus = Plus(1);
assert(plus(42) == 43);
```

Equivalent to Lambda:
```cpp
auto plus = [value=1](int x) {return x + value;};
assert(plus(42) == 43)
```

Lambda is a syntax sugar to create a functor which captures local variable where it's defined in, also overload () operator so that it's callable.

That is basically **Closures** without introducing: heap-allocation , garbage collection , runtime polymorphism ...





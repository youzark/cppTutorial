# Exception Handling and Exception Safety

<!--toc:start-->
- [Exception Handling and Exception Safety](#exception-handling-and-exception-safety)
  - [1. What is Exceptions Designed for?](#1-what-is-exceptions-designed-for)
    - [1.1 EH is for synchronous Program Errors](#11-eh-is-for-synchronous-program-errors)
    - [1.2 What is C++ Exception Handling designed for?](#12-what-is-c-exception-handling-designed-for)
    - [1.3 Error Handling without Exception](#13-error-handling-without-exception)
      - [1.3.1 C style](#131-c-style)
      - [1.3.2 Problem of Returning Error Indicators](#132-problem-of-returning-error-indicators)
  - [2. Exception Handling](#2-exception-handling)
      - [2.1 Unwinding the Stack](#21-unwinding-the-stack)
    - [2.2 Exceptions and Interfaces](#22-exceptions-and-interfaces)
    - [2.3 noexcept](#23-noexcept)
    - [2.4 Handlers with ellipses](#24-handlers-with-ellipses)
  - [3. Exception Safety](#3-exception-safety)
    - [3.1 not safe code:](#31-not-safe-code)
    - [3.1 Achieve Basic Exception Safety Guarantee with RAII](#31-achieve-basic-exception-safety-guarantee-with-raii)
      - [3.1.1 Failure in Constructors](#311-failure-in-constructors)
      - [3.1.2 Basic Guarantee](#312-basic-guarantee)
    - [3.2 To achieve Strong Guarantee](#32-to-achieve-strong-guarantee)
    - [3.3 The Nothrow Guarantee](#33-the-nothrow-guarantee)
      - [3.3.1 what if Destructor can throw](#331-what-if-destructor-can-throw)
      - [3.3.2 Why no throw swap](#332-why-no-throw-swap)
  - [4. When Should an Exception be Thrown?](#4-when-should-an-exception-be-thrown)
<!--toc:end-->

## 1. What is Exceptions Designed for?

### 1.1 EH is for synchronous Program Errors

- synchronous program errors:
    - resource allocation failure
    - value out of range
    - ...
- asynchronous events
    - hardware interrupts
    - instruction traps
    - ...

### 1.2 What is C++ Exception Handling designed for?

Exception Handling is a mechanism to report Error that can decouples error reporting from error handling easily.

### 1.3 Error Handling without Exception

#### 1.3.1 C style

return error Indicators or set some global to indicate failure.
![CErrorHandling](/home/youzark/projects/cppTutorial/language/exception/exception2019/img/CErrorHandling.png)

#### 1.3.2 Problem of Returning Error Indicators

The goal here is to separate error report and error handling.

- If the logic that respond to a certain failure resides far from the code that triggers the failure, it's hard to trace down where the failure is handled, cause the Error Indicator might be returned multiple times before it reached the function that's capabile of dealing it. which makes the code hard to maintain.

- It's fragile: error code is passed in a chain for function calls. With multiple error types and function calls, these chains forms a net . If any node function on the net that's not responsible for dealing with the error forget to pass it upword will cause unhanddled error.


## 2. Exception Handling

The primal purpose is to hidden the path through which error signal travels, make it as if the error goes directly from the reporter to the dealer.

* Reporter side:(throw by value)  
    - throw exception-expression  

* Catching side:(catch by reference)

    - catch(exception-declaration) { }  
    - try { statement-sequence }  
    catch(exception-declaration) { statement-sequence }

> Headers that define comman exceptions:  
> \<exception\> & \<stdexcept\>  
> \<new\> and \<typeinfo\> also define some exception


#### 2.1 Unwinding the Stack

If one function throws a exception , down the function call stack, if one function contain the corresponding catch block,execution continues from the catch block ,else the local variable of that function will get destructed and return to the higher level caller in search of catch block.

program terminate if main throws an unhandled exception.

### 2.2 Exceptions and Interfaces

1. When you design a function's interface, decide what it's allowed to throw.

2. Be cautious about changing what a function can throw.


### 2.3 noexcept

1. noexcept is not enforced at compile time
2. If a noexcept do throw, terminate() is called

### 2.4 Handlers with ellipses

catch (...) {
    delete p;
    throw; // rethrow whatever I caught before
}

Not trying to handle exception ,it try to envoke stack unwinding and release some resource

## 3. Exception Safety

keep program consistent even if a exception happens

1. The Basic Guarantee      -- resource safe and invariant kept
2. The Strong Guarantee     -- commit and roll back
3. The Nothrow Guarantee    -- no exception will ever propagate from the function(deal with all exception internally)

### 3.1 not safe code:
```cpp
int g() {return 0;}

void f_not_exception_safe(char const* name) {
    FILE *outf = fopen(name, "w");
    if (outf != nullptr) {
        fprintf(outf, "The value is: %d",g());
        fclose(outf); // if g() throws a exception, outf get deleted in stack unwinding , but file is not closed.
    }
}

```

### 3.1 Achieve Basic Exception Safety Guarantee with RAII
```cpp
class file {
    public:
        file(char const *name, char const *mode);
        ~file() noexcept;
        file(file const&);
        file(file &&);
        file& operator=(file const&);
        file& operator=(file &&);
        bool is_open() const noexcept;
        void put(int i);
        void put(char const *);
        // ...
    private:
        FILE *pf;
};

file::~file() noexcept {
    if(pf != nullptr) {
        fclose(pf);
    }
}
```

#### 3.1.1 Failure in Constructors

Solution one: throw
```cpp
file::file(char const *name, char const *mode):
    pf {fopen(name, mode)}
{
    if(pf == nullptr) {
        throw "Some exception represent file cannot be opened";
    }
}
```
1. file's constructor doesn't finish , no object is created actually.(thus no destructor will get called)
2. The constructor might need to clean up other resources before it throws. (usually a better approach is one class for one resource)


Solution two: Check before use
```cpp
inline bool file::is_open() const noexcept {
    return pf != nullptr;
}
```

#### 3.1.2 Basic Guarantee
```cpp
void f_basic_exception_safe(char const *name) {
    file outf{name,"w"};
    if (outf.is_open()) {    // assume we use solution two.
        outf.put("The value is: ");
        outf.put(g());
    }
}
```
The file is always closed:
1. if constructor failed: no file opened
2. if g() throws , file will get close by ~file() in stack unwinding.

### 3.2 To achieve Strong Guarantee

Break function into two parts:
1. do every thing that might throw locally(only use local variable which will get deleted in stack unwinding)
2. Update state of the program using only Nothrow operations

```cpp
void file::put(char const*) noexcept;
bool file::is_open() const noexcept;

void f_strong_exception_safe(char const *name) {
    // Part One:
    int temp = g();                 // operations that can throw
    file outf{name,"w"};

    // Part Two:
    if (outf.is_open()) {
        outf.put("The value is:");  // operations that never throw
        outf.put(temp);
    }
}
```

### 3.3 The Nothrow Guarantee

Must no throw functions:
1. destructors
2. operator delete
3. swap member functions

#### 3.3.1 what if Destructor can throw

```cpp
void read_two_files(char const *f1,char const*f2) {
    file first(f1,"r");
    file second(f1,"r"); // throws exception

    // some logic here
    
    // second.~file()
    // first.~file()    // throws yet another exception during stack unwinding
}
```
C++ allows only one exception to be thrown at once.

So operations along stack unwinding should never throw otherwise will cause program prematurely terminate.

#### 3.3.2 Why no throw swap

1. To achieve Strong Exception Safety: We need an operation that can update program state while never throw.
2. Easily Implemtation: 
    1. swap builtin types never throw.
    2. swap class object can be tore down into swap builtin types and subclasses


## 4. When Should an Exception be Thrown?
exception throw behave kind of like goto and also brings heavy overhead.

Only for really exceptional cases!!! (no local or by near caller solution possible ,and cannot be ignored)



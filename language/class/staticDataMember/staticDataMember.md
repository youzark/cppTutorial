# Static Data Member & Const Static Data Member

## Part One : Intuition

### Non-const Static Data Member

Static Data Member is usually used to store attribute that's shared by all instances of a class.  

For instance, we want to introduce a log system to out program, with witch we can set log level to change the global 
behaviour.(actually the best practice here is to use enum hack).
* 0:"ERROR" 
* 1:"DEBUG"
* 2:"INFO"

That requires this variable shared globally by all Logger instances ,then we should qualify it with static. That's the major use-case of static data member.

### const static Data Member
Const Static Data Member is the recommanded way to define constant in class scope:
* compaired with Macro: 
    * Macro doesn't respect scope which will lead to potential naming collisions and unexpected behaviour
    * when defined with const static, compiler awares of it's existance thus can perform better optimization

```cpp
#define LOGGER_VERSION 1

class Logger {
public:
    const static int LoggerVersion = 1; // respect scope
};
```
* compaired with const:
    * only one copy of const static variable will be stored thus is more memory efficient.
    * const static integral variable will get inlined at compile time and doesn't take memory at all.
```cpp
class Logger {
public:
    const int LoggerVersion = 1; // every instance of Logger will have a copy of LoggerVersion
    const static int s_LoggerVersion = 1; // get inlined at compile and takes no memory
};
```

## Part Two : Language Specification
### 1. Declaration and Definition

* You can only write declaration of static data member in class definition.
* Definition(memory allocation & value assignment) of static data member should be located outside of the class definition.
* To fully understand difference between declaration and definition of variable , please refer to [difference between declaration and definition](../../memory/variable/initializationAndDeclaration.md). In short, declaration doesn't involve memory allocation and usually doesn't involve Initialization.
----

In the following code blocks:
we define a Logger to demostrate the definition and declaration of **static member variable** and **const static member variable**

#### 1.1 Non-const static

```cpp
class Logger {
public:
    static int LogLevel; // declaration of LogLevel
    Log() {
        std::cout << "Log constructor Called" << std::endl;
    }
}

int Logger::LogLevel = 0; // definition of LogLevel
```

#### 1.2 const static

Things are slightly different for **const static variable**

1. For integral type( int ,char ,long, bit fields ... ), you are allowed to initialize const static variable within class declaration. But it's still considered a declaration ,which meaning no memory is allocated for that variable. Every use of that variable get inlined into the surrounding code.

```cpp
class Logger {
public:
    const static int constLogLevel = 10; // Valid ,But alghouth constLogLevel is initialized here
                                         // This statement is still considered declaration
                                         // which means without proper definition 
                                         // we cannot access address of constLogLevel
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};
int main() {
    std::cout << Logger::constLogLevel << std::endl;  // Valid: output 10
    std::cout << &Logger::constLogLevel << std::endl; // ERROR: alghouth declaration provide initialization
                                                      // constLogLevel is not stored in Memory before definition
}

```

2. to make const static class member(integral) a valid variable(addressable) , you have to separate declaration and 
provide definition for const variable:
   
```cpp
class Logger {
public:
    const static int constLogLevel; // Notice: this time no init value
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};
const int Logger::constLogLevel = 9; // Definition of constLogLevel
                                     // Note here: const quailfier is a must(and must in global scope)
int main() {
    std::cout << Logger::constLogLevel << std::endl;  // Valid: output 9
    std::cout << &Logger::constLogLevel << std::endl; // Now ,it's valid to access address of constLogLevel
}
```

3. For non-integral type const static data member(float , user-defined type ...), you are not allowed to initialized 
at class definition( they cannot be inlined at compile time )
```cpp
class MsgFormatter {
    MsgFormatter() {
        std::cout << "MsgFormatter constructor Called" << std::endl;
    }
};
class Logger {
public:
    const static int constLogLevel = 10; // Valid ,integral const static can be initialized 
    const static MsgFormatter formatter{}; // ERROR: non-integral const static must be 
                                           // initialized out of class definition
    const static float constFloat = 1.0; // ERROR: same reason for formatter(non-integral)
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};
```
to make things work , we have to separate definition and declaration of non-integral const static data member:
```cpp
class Logger {
public:
    const static MsgFormatter formatter; // Valid: declaration of formatter
    const static float constFloat; // Valid: declaration of constFloat
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};
const MsgFormatter Logger::formatter{}; // Definition of formatter
const float Logger::constFloat = 1.0; // Definition of constFloat

```

#### 2. Memory, Scope and Lifetime

##### 2.1 in memory
* stored in DATA Segemnt(.BSS , .DATA).
* initialized prior to program execution(thus must give definition in global scope), but initialization order between statics is not guaranteed.So don't use static variable to intialize static variable.
* Only one copy of that member is store for the entire class and is shared by all the instances of that class.

##### 2.2 Scope
Class Scope

##### 2.3 Lifetime
the lifetime of the program

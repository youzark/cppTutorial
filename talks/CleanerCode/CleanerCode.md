[source vid](https://www.youtube.com/watch?v=9ch7tZN4jeI) 

# 

## 0. Intro instance: Implementation of STL

Pros:
* well designed API
* Consistant
* Efficient
* complies with core guidelines

Cons:
* long Implementation
* cyclomatic complexity is high
* long header
* excessive use of macros and compilation flags
* repetitive

Clean code means differently in different context.

## 1. Clean Code in Context

### 1.1 Clean Code Tradeoffs

Core Library Code  ->  Project Library Code  ->  Components

down the stack : stability , backward compatibility , multi platform support and efficiency get more and more important

up the stack : readibiliy get more and more important


**Not** necessarily learn clean code from standard library , cause the requirement is different

## 2. Best Practices for C++ Programming

### 2.1 General Best Practices
1. Single Responsibility Principle
2. Open Closed Principle
3. Liskov Substitution
4. Interface Segregation
5. Dependency Inversion Principle

### 2.2 Best Practices for C++

#### 2.2.1 C++ Core Guidelines
[C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main) 

It's recommanded to use as a referenece, but you should shim throught it to get the philosophy of this document

## 3. Develop an application and clean it

### 3.1 Problem Setup

[git repo]() 

VOD(video on Demand) : server , client for test , google test , Resharper ...

### 3.2 Managing headers

* header should include all headers it depends on
* A header should not include any headers it does not depend on 
* Use pimpl or an interface to represent dependencies to avoid needing to include implementation headers
* separate internal headers from API headers
* Use modules!

### 3.3 Error Handling

Guidelines:
1. Use exception for the exceptional
2. Use expected for the expected ( in C++23 ) (exception that are expected)

std::expected<T,E> 

    T : type of the expected value    
    E : error type used when unexpected outcome occurs

### 3.4 Use generators for lazy iteration






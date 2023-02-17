# Function and Class Templates
[source video](https://www.youtube.com/watch?v=LMP_sxOaz6g&list=PLHTh1InhhwT4CTnVjJqnAKeMfGzOWjsRa&index=2) 

## Function Templates

> function template declaration
```cpp  
template <typename T>
void swap(T &a,T &b);
```

> function template definition
```cpp
template <typename T>
void swap(T &a, T&b) {
    T temp{a};
    a = b;
    b = temp;
}
```

### Templates parameter

> Templates parameters v.s. function parameters

* type parameter
    - type parameter T is a placeholder for a type argument.
    - substitution happens at **compile time** 
* function parameter
    - pass at **compile time** 
    - pass at **run time** 

> template type parameter scope
```cpp
template <typename T>   //T's Scope start here
void swap(T &a, T &b) {
    T temp{a};
    a = b;
    b = temp;
}                       //T's ends here
```

### Template Instantiation

> The act of generating a function definition from a template
```cpp
int i,j;
swap<int>(i, j);
void swap<int>(int &a, int &b);             // Generated function declaration(If function is not inline)

string s, t;
swap<string>(s, t);
void swap<string>(string &a, string &b);    // Same process

char const *p, *q;
swap<char const *>(p, q);
void swap<char const *>(char const *&a, char const *&b);
```

> No duplicate or unnecessary instantiation will happen.

> typename <==> class:  In template parameter list


## Class Templates

> generalization of an object type

```cpp
template<typename P>
class rational {
public:
    rational();
    rational(P n);
    rational(P n, P d);

    rational& operator+=(rational const& ro);
    rational& operator-=(rational const& ro);
private:
    P num, den;
};
```

> Instantiated class

type parameter is part of type name of that class
```cpp
rational<long> rl{10};          // rational<long> is the full type name of the instantiated class

typedef rational<int> irat;     // To make things easier, you can def alias
using irat = rational<int>;     // Same as typedef

```

### Member Functions
```cpp
template<typename P>
class rational {
public:
    rational();
    rational(P n);
    rational(P n, P d);

    rational& operator+=(rational const& ro);
    rational& operator-=(rational const& ro);
private:
    P num, den;
};

template<typename T>
rational<T>::rational(T n) { };
rational<T>::operator+=(rational const& ro);
```
> \<T\> is optional inside class definition: rational means rational\<T\> inside class scope

Notice to define a member funtion outside of class template definition:  
1. class name: rational\<T\> must be full class name.
2. type name inside class can omit \<T\>

![omitTypeName](/home/youzark/projects/cppTutorial/language/template/functionAndClass/img/omitTypeName.png)
![typenameNotAllowed](/home/youzark/projects/cppTutorial/language/template/functionAndClass/img/typenameNotAllowed.png)

### Static Data Members

> Before c++17, it's a must to separate static data member declaration and definition.

```cpp
template<typename T>
class gadget {
    static unsigned long counter;   
};

template<typename T>
unsigned long gadget<T>::counter = 10;
```
* Each instantiation of gadget<T> gets a distinct instance of counter
* Only declaration of counter instantiated rather than definition of counter


```cpp
template<typename T>
class gadget {
    inline static unsigned long counter = 10;
};
```
With inline , you are allowed to give definition of static data member within class 
definition.






















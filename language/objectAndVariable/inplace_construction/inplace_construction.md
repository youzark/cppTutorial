# In-Place Construction
[source vid](https://www.youtube.com/watch?v=oTMSgI1XjF8&list=PLHTh1InhhwT4CTnVjJqnAKeMfGzOWjsRa&index=6) 

## 1. What happens when we move something

> Moving from a string usually isn't any faster than copying from it.

Copy can be slightly faster than move if the string is small enought for small string optimization.

![stringCopyAndMove](/home/youzark/projects/cppTutorial/language/objectAndVariable/inplace_construction/img/stringCopyAndMove.png)

> moves aren't necessarily cheap!

## 2. Copy elision(RVO: return value optimization)

> What's copy elision?  
> Construct return value directly in caller's stack

![copyElision](/home/youzark/projects/cppTutorial/language/objectAndVariable/inplace_construction/img/copyElision.png)

> When can RVO apply?
* return a temporary(prvalue) 
* return the name of a stack variable(not parameter)

> When RVO cannot apply?
* If there is no opportunity to
    * the return value is parameter. Cause the callee get no change to Construct what's passed in.
    ```cpp
    std::string echo(std::string s) {
        return s;
    }
    ```
* If it's not of the right type
```cpp
std::string wrongType() {
    std::string s = "No RVO";
    return std::move(s);
}
```
* If the callee doesn't know enought
```cpp
std::string undecided() {
    std::string foo = "foo";
    std::string bar = "barbar";
    if(somecondition) return foo;
    else return bar;
}
```




## 3. Putting Stuff into a vector
## 4. Initializer_list
## 5. Putting Stuff into a map
## 6. Putting Stuff into other things
## 7. Final thoughts







































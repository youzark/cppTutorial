[source video](https://www.youtube.com/watch?v=0ojB8c0xUd8)

# Exception Handling

## 1. The Exception Situation

* Many Code guidelines and projects ban exceptions.But why?

### 1.1 defact of Exceptions

#### 1.1.1 Performance Overhead

![PerformanceOfExceptions](/home/youzark/projects/cppTutorial/language/exception/img/PerformanceOfExceptions.png)
The Yellow Bar represent overhead of exceptions

#### 1.1.2 Exceptions make it harder to reason about functions

#### 1.1.3 Exceptions rely on dynamic memory

Exception might need dynamic size , Unpredictible times of exceptions might happen

#### 1.1.4 Exceptions make the binary size grow(not zero overhead)

### 1.2 Exceptions might be better even perfect

One Paper [Zero-overhead deterministic exceptions: Throwing values](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2232r0.html) point out the drawback in the previous discussion can be overcomed.

## 2. How do Exception work

```cpp
void f()
{
    std::string s{"Some Default initializer"};
    if(/* some condition */) {
        throw std::runtime_error("...");
    }
    /* something else */
}
void g()
{
    std::vector<int> v{1,2,3,4,5,6};
    /* something else */
    f();
    /* something else */
}
int main() {
    try {
        g();
        h();
    }
    catch {
        /* something else */
    }
}
```

### 2.1 Stack unwinding
When exception got catched:
1. Objects on the stack get released
2. Destruction happened in reverse order(to function call stack)

### 2.2 Unhandled exceptions
std::terminate() is call as a result, which will cause:
1. no stack unwinding
2. no destructors are called
3. Resources are potentially leaked

So at least catch( ... ) {} should be used

## 3. Best Practices of Exception Handling

### 3.1 When to use exceptions
1. for errors that are expected to occur rarely(You don't really care about performance)
2. exceptions cases that cannot be dealt with locally (I/O )
3. for operators and constructors(when you are not allow to return error code)

### 3.2 When not to use exceptions
1. errors that are expected to occur frequently(when preformance matters)
2. for functions that are expected to fail:
```cpp
auto to_int(std::string const& s ) -> std::optional<int>
// it's common that this functions fails.

auto to_int(std::string const& s ) -> std::expected<int>
// better way to signal that some error might happen
```
3. if you have to guarantee certain response times
4. for things that should never happen: Bugs
    1. Dereference nullptr
    2. Out-of-range access
    3. Use after free

### 3.3 How to use Exceptions

#### 3.3.1 Build on the std::exception Hierarchy
![exceptionHierarchy](/home/youzark/projects/cppTutorial/language/exception/img/exceptionHierarchy.png)

#### 3.3.2 Throw By rvalue

```cpp
void f() {
    if(/*condition*/) {
        std::string errorMsg = "Error message"
        throw std::runtime_error(errorMsg); // why need a name for error?
    }
}
```
Don't throw by pointer or reference

#### 3.3.3 Catch by Reference(const)
1. to save memory and performace
2. to prevent potentially slicing the objects

### 3.4 The Exception Safety Guarantee

#### 3.4.1 Basic Exception Safety Guarantee

1. Invariants are preserved
2. No resources are leaked

#### 3.4.2 String Exception Safety Guarantee

1. Invariants are preserved
2. No resources are leaked
3. No state change (commit-or-rollback)
4. Not always possible (e.g. sockets, streams,etc)

#### 3.4.3 No_Throw Guarantee

operations are not allowed to fail. Use noexcept

### 3.5 How to write Good Code

#### 3.5.1 Exception unsafe code E.g.
```cpp
class Resource{};

class Widget {
private:
    int i{0};
    std::string s{};
    Resource* pr{}; // might be nullptr
public:
    Widget( Widget const& w ):
        i (w.i),
        s (w.s)
    {
        if(w.pr) pr = new Resource( *w.pr );
    }
    // too difficult to comprehend
    Widget& operator=( Widget const& w ) {
        // self assignment
        if( this == &w ) return *this;

        i = w.i;
        s = w.s;
        delete pr; // Possibily Dangling Pointer if new failed, Invariant Broken!
                   // delete will free the resources ,but will not refresh the pointer
        if( w.pr ) pr = new Resource( *w.pr ); // Both new and the Resource Constructor could throw!
        else pr = nullptr;

        return *this;
    }
};
```

#### 3.5.2 Basic Exception Safety
A quick fix can be assign possibly dangling pointer to nullptr beforehand.
```cpp
class Resource{};

class Widget {
private:
    int i{0};
    std::string s{};
    Resource* pr{}; // might be nullptr
public:
    Widget( Widget const& w ):
        i (w.i),
        s (w.s)
    {
        if(w.pr) pr = new Resource( *w.pr );
    }
    // too difficult to comprehend
    Widget& operator=( Widget const& w ) {
        // self assignment
        if( this == &w ) return *this;

        i = w.i;
        s = w.s;
        delete pr; // Possibily Dangling Pointer if new failed, Invariant Broken!
                   // delete will free the resources ,but will not set the pointer
        pr = nullptr; // Notice , small changes saves the day
        if( w.pr ) pr = new Resource( *w.pr ); // Both new and the Resource Constructor could throw!

        return *this;
    }
};
```

This trick works ,however , relies heavily on human decision and thus extremely delicate
A better Solution should be:
```cpp
class Resource{};

class Widget {
private:
    int i{0};
    std::string s{};
    std::unique_ptr<Resource> pr{}; // might be nullptr

public:
    Widget( Widget const& w ):
        i (w.i),
        s (w.s)
    {
        if(w.pr) pr = std::make_unique<Resource>( *w.pr );
    }
    // too difficult to comprehend
    Widget& operator=( Widget const& w ) {
        // self assignment
        if( this == &w ) return *this;

        i = w.i;
        s = w.s;

        if( w.pr ) pr = std::make_unique<Resource>( *w.pr );

        return *this;
    }
};
```
The philosophy behind this code snippet is exactlly RAII

However make_unique might still fail!!! Before which the state of the object has been changed.

#### 3.5.3 the code respect "Strong Exception Safety Guarantee"
```cpp
class Resource{};

class Widget {
private:
    int i{0};
    std::string s{};
    std::unique_ptr<Resource> pr{}; // might be nullptr

public:
    Widget( Widget const& w ):
        i (w.i),
        s (w.s)
    {
        if(w.pr) pr = std::make_unique<Resource>( *w.pr );
    }
    // too difficult to comprehend
    Widget& operator=( Widget const& w ) {
        // self assignment
        if( this == &w ) return *this; // this line become optional

        // move template idiom : It work assume move assigment operator should be noexcept
        Widget temp( w ); // if constructor throw exception , we still not touch *this yet
        *this = std::move(temp);

        return *this;
    }
    Widget& operator=( Widget && w ) noexcept;
```

"Strong Exception Safety Guarantee" relays heavily on "No Throw Exception Guarantee"

#### 3.5.4 function that **MUST** noexcept

* destructors
* move
* swap

This function are considered noexcept by Default by the compiler

#### 3.5.5 The benefit of noexcept
1. promise to never throw
2. faster ( less statement by compiler )
3. if an exception leaves a function marked with noexcept , terminate() is called
**Not Checked by Compiler**
4. A noexcept promise cannot be taken back

### 3.6 How to Deal with Failing Cleanup Code

Sometimes ,we might fail when releasing resources ( like a file ), what to do?

no good answer , but don't throw.


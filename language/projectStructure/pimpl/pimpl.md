[source doc](https://en.cppreference.com/w/cpp/language/pimpl) 
# pImpl Idiom

## Intuition:
It's reasonable when the interface of an API changes , the user code of that API need to recompile(probably need rewrite)
But it's not that reasonable when the interface(public Part) of an API remain untouched, the implementation details
(usually in private part) change, the user code need to recompile , so we need to remove this compilation dependency.

1. hide private member data and member function(including their layout) from API user

2. remove compilation dependency : layout of the class definition won't change as
implementation of class changes. thus users of that class needless to recompile as 
the implementation changes.(Used in ABI: application binary interface)

```cpp
// interface (widget.h)
class widget
{
    // public members
private:
    struct impl; // forward declaration of the implementation class
    // One implementation example: see below for other design options and trade-offs
    std::experimental::propagate_const< // const-forwarding pointer wrapper
        std::unique_ptr<                // unique-ownership opaque pointer
            impl>> pImpl;               // to the forward-declared implementation class
};
 
// ---------------------------
// implementation (widget.cpp)
struct widget::impl
{
    // implementation details
};
```

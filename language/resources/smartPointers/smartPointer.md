# Smart Pointers
[source vid](https://www.youtube.com/watch?v=xGDLkt-jBJ4&list=PLHTh1InhhwT4CTnVjJqnAKeMfGzOWjsRa&index=8) 

## unique_ptr
- unique_ptr mamage deletion of the object it point to.
- unique_ptr is moveable and is move-only
- std::unique_ptr<T[]> manage array of elements
- template of unique_ptr has "Allocator" and "Deletor" parameter

```cpp
namespace std {
    template<typename T>
    struct default_delete {
        void operator ()(T *p) const {
            delete p;
        }
    };
    template<typename T,typename Deleter = std::default_delete<T>>
    class unique_ptr {
    private:
        T *p_ = nullptr;
        Deleter d_;
    public:
        ~unique_ptr() {
            if(p_) d_(p_);
        }
        unique_ptr(T const*);
    };
}
```

### custome Deleter when the managed resource that is not heap memory

```cpp
struct FileCloser {
    void operator()(FILE *fp) const {
        if( fp ) {
            fclose(fp);
        }
    }
};
int main() {
    FILE *fp = fopen("input.txt","r");
    std::unique_ptr<FILE,FileCloser> uptr{fp};
}
```

### Rules of thumb for smart pointers

1. Treat Smart-pointer types just like raw pointer types
    - pass by value
    - Return by value
    - Passing a pointer by reference is usually a code smell
    - Same goes for smart pointer
2. A function taking a unique_ptr by value shows **tranfer of ownership** 
    - also the responsity to "call" deleter
3. Smart pointer are frequently implementation details and glue
    - use unique_ptr to implement resource management rather than implement by oneself


## shared_ptr

shared ownership <===> reference counting

> The last one that leaving the room is responsible for turning out the light.

Code that mimic this behaviour
```cpp
struct Widget {
    std::atomic<int> usecount_{1};
    Widget *retain() { ++usecount_; return this; }
    void release() {if (--usecount_ == 0 ) delete this;}
};
```

### Mechanism behind shared_ptr<T>

![shared_ptr](/home/youzark/projects/cppTutorial/language/resources/smartPointers/img/share_ptr.png)
![two_shared_ptr](/home/youzark/projects/cppTutorial/language/resources/smartPointers/img/two_share_ptr.png)

* why the control block need a pointer to controlled object:  
    - the pointer in the control block actually point to the derived class rather than the base class.  
    - the pointer in shared_ptr<T> may point to the base class , and delete on it will cause slicing. So it has nothing to do with ownership management.

### Aliasing constructor
![aliasingConstructor](/home/youzark/projects/cppTutorial/language/resources/smartPointers/img/aliasingConstructor.png)


## Prefer std::make_unique & std::make_shared over new

new must have a corresponding delete.
use std::make_unique and std::make_shared let us not touch new and delete 

### make_share can even be optimized
![optimization_by_make_share](/home/youzark/projects/cppTutorial/language/resources/smartPointers/img/optimization_by_make_share.png)

unique_ptr<T> is implicitly convertible to shared_ptr<T>


## Use std::weak_ptr for shared_ptr-like pointers that can dangle

### Dangling weak_ptr
![danglingWeak_ptr](/home/youzark/projects/cppTutorial/language/resources/smartPointers/img/danglingWeak_ptr.png)

### What's weak_ptr any way
You can't dereference a weak_ptr. So it's not actually a pointer!  

* Think of a weak_ptr as a "ticket for a share_ptr".
    - If you hold a weak_ptr , you're entitled to reveive a shared_ptr possibily
    - If you hold a shared_ptr, you're certainly entitled to get a weak_ptr
* The "redeem a ticket" operation:
    - explicit type-conversion
    - weak_ptr.lock()   // return a share_ptr
* The "get a ticket" operator
    - explicit conversion

```cpp
void recommended(std::weak_ptr<T> wptr) {
    std::share_ptr<T> sptr = wptr.lock();
    if( sptr != nullptr ) {
        use(sptr);
    }
}

void not_recommanded(std::weak_ptr<T> wptr) {   // No reason to use this.
    try {
        std::share_ptr<T> sptr { wptr };        // never return nullptr, just throw
        use(sptr);
    } catch (const std::bad_weak_ptr&) { }
}

if ( auto sptr = wptr.lock() ) {    // this idiom test for nullptr automatically
    use(sptr);
}
```

### What's weak_ptr for?
[source post](https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful) 

Suppose you have Team and Member objects.

Obviously it's a relationship : the Team object will have pointers to its Members. And it's likely that the members will also have a back pointer to their Team object.

Then you have a dependency cycle. If you use shared_ptr, objects will no longer be automatically freed when you abandon reference on them, because they reference each other in a cyclic way. This is a memory leak.

You break this by using weak_ptr. The "owner" typically use shared_ptr and the "owned" use a weak_ptr to its parent, and convert it temporarily to shared_ptr when it needs access to its parent.

```cpp
weak_ptr<Parent> parentWeakPtr_ = parentSharedPtr; // automatic conversion to weak from shared

shared_ptr<Parent> tempParentSharedPtr = parentWeakPtr_.lock(); // on the stack, from the weak ptr
if( !tempParentSharedPtr ) {
  // yes, it may fail if the parent was freed since we stored weak_ptr
} else {
  // do stuff
}
// tempParentSharedPtr is released when it goes out of scope

```

weak_ptr points to resource that can expire without you. But you still hold the posibility to visit it if not expired.

### Use enable_shared_from_this when an object is its own ticket




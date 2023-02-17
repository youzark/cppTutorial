#include <concepts>
#include <memory>
#include <string>
#include <utility>

template<typename T>
class vector
{
    vector& operator=(const vector& rhs);
    vector& operator=(vector &&);
};

template< typename T >
std::remove_reference_t<T>&&
move( T && t ) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}

class Widget_default_movable {
private:
    int i {0};
    std::string s{};
    std::unique_ptr<int> pi{};
public:
    Widget_default_movable(Widget_default_movable && w) = default;
    Widget_default_movable& operator=(Widget_default_movable && w) = default;
};

class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};
public:
    Widget( Widget&& w ):
        i ( std::move(w.i) ),
        s ( std::move(w.s) ),
        pi ( std::exchange(w.pi , nullptr) )
    {
    }

    Widget& operator=( Widget && rhs ) {
        // self-move-assignment(cause by state oneself movable, it's treated as a expiring value) is different to self-copy-assignment
        delete pi;              //cause the resources is managed by raw pointer, we need to delete it whenever the one 
                                //responsible for manage it point to something else
        i = std::move(rhs.i);
        s = std::move(rhs.s);
        pi = std::exchange(rhs.pi,nullptr);     //To avoid double deletion problem.
        
        return *this;
    }

    // Copy-Move assignment
    Widget& operator=( Widget w ) 
    {
        swap( w );
        return *this;
    }
    void swap( Widget& w ) {
        // use std::swap member wise.
    }
};

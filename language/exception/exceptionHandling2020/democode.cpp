#include <string>

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

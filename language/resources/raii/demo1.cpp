#include <algorithm>
#include <utility>
#include <cstddef>

class NaiveVector {
    int *ptr_;
    size_t size_;
public:
    // constructor
    NaiveVector():
        ptr_(nullptr),
        size_(0)
    {
    }

    // Copy constructor
    NaiveVector(NaiveVector const& rhs) {
        ptr_ = new int[rhs.size_];
        size_ = rhs.size_;
        std::copy(rhs.ptr_,rhs.ptr_ + rhs.size_,ptr_);
    }

    // Destructor
    ~NaiveVector() {
        delete [] ptr_;
    }

    // move constructor
    NaiveVector(NaiveVector&& rhs) noexcept {
        ptr_ = std::exchange(rhs.ptr_,nullptr); // assign to nullptr so that the original destructor won't touch the 
                                                // resources
        size_ = std::exchange(rhs.size_,0);
    }

    void swap(NaiveVector& rhs) noexcept {
        using std::swap;
        swap(ptr_,rhs.ptr_);
        swap(size_,rhs.size_);
    }

    friend void swap(NaiveVector& a, NaiveVector& b) { // support std::swap(NaiveVector&,NaiveVector&) overload
        a.swap(b);
    }

    // copy assignment 
    NaiveVector& operator=(NaiveVector const& rhs) {
        NaiveVector copy{rhs};
        copy.swap(*this);
        return *this;
    }

    NaiveVector& operator=(NaiveVector && rhs) {
        NaiveVector copy{std::move(rhs)};
        copy.swap(*this);
        return *this;
    }

    void push_back(int newvalue) {
        int *newptr = new int[size_ + 1];
        std::copy(ptr_,ptr_ + size_,newptr);
        delete [] ptr_;
        ptr_ = newptr;
        ptr_[size_++] = newvalue;
    }

};

struct RAIIPtr {
    int *ptr_;
    RAIIPtr(int *p) : ptr_(p) {};
    ~RAIIPtr() { delete [] ptr_; }

    RAIIPtr(RAIIPtr const&) = delete;
    RAIIPtr& operator=(RAIIPtr const&) = delete;
};

int main() {


}

#include <atomic>
#include <cstdio>

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

    struct Widget {
        std::atomic<int> usecount_{1};
        Widget *retain() { ++usecount_; return this; }
        void release() {if (--usecount_ == 0 ) delete this;}
    };

}

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

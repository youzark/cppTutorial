#include <cstdio>

int g() {return 0;}

void f_not_exception_safe(char const* name) {
    FILE *outf = fopen(name, "w");
    if (outf != nullptr) {
        fprintf(outf, "The value is: %d",g());
        fclose(outf); // if g() throws a exception, outf get deleted in stack unwinding , but file is not closed.
    }
}

class file {
    public:
        file(char const *name, char const *mode);
        ~file() noexcept;
        file(file const&);
        file(file &&);
        file& operator=(file const&);
        file& operator=(file &&);
        bool is_open() const noexcept;
        void put(int i);
        void put(char const *);
        // ...
    private:
        FILE *pf;
};

file::file(char const *name, char const *mode):
    pf {fopen(name, mode)}
{
    if(pf == nullptr) {
        throw "Some exception represent file cannot be opened";
    }
}

file::~file() noexcept {
    if(pf != nullptr) {
        fclose(pf);
    }
}

inline bool file::is_open() const noexcept {
    return pf != nullptr;
}

void f_basic_exception_safe(char const *name) {
    file outf{name,"w"};
    if (outf.is_open()) {
        outf.put("The value is: ");
        outf.put(g());
    }
}

void f_strong_exception_safe(char const *name) {
    // Part One:
    int temp = g();                 // operations that can throw
    file outf{name,"w"};

    // Part Two:
    if (outf.is_open()) {
        outf.put("The value is:");  // operations that never throw
        outf.put(temp);
    }
}

void read_two_files(char const *f1,char const*f2) {
    file first(f1,"r");
    file second(f1,"r");

    // some logic here
    
    // second.~file()
    // first.~file()
}

#include <algorithm>
#include <utility>
#include <memory>

class Vec {
    std::unique_ptr<int[]> m_uptr; // unique_ptr respect rule of five (delete copy assignment)
    int m_size;

public:
    Vec();
    Vec(const Vec& rhs) {
        m_uptr = std::make_unique<int[]>(rhs.m_size);
        m_size = rhs.m_size;
        std::copy(rhs.m_uptr,rhs.m_uptr + m_size, m_uptr);
    }
    ~Vec() = default;
    Vec(Vec&&) noexcept = default;
    void swap(Vec& rhs) noexcept {
        using std::swap;
        swap(m_uptr, rhs.m_uptr);
        swap(m_size, rhs.m_size);
    }
    friend void swap(Vec& a,Vec& b) noexcept {
        a.swap(b);
    }
    Vec& operator=(const Vec& rhs) {
        Vec copy{rhs};
        copy.swap(*this);
        return *this;
    }
};


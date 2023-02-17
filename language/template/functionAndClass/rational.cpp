template<typename P>
class rational {
public:
    rational();
    rational(P n);
    rational(P n, P d);

    rational& operator+=(rational const& ro);
    rational& operator-=(rational const& ro);
private:
    P num, den;
};

template<typename T>
rational<T>::rational(T n) { };

template<typename T>
class gadget {
    inline static unsigned long counter = 10;
};

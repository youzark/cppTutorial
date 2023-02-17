#ifndef CLASS_DOG
#define CLASS_DOG
#include <iostream>
#include <string>

class dog
{
public:
    dog() = default;
    dog(dog &&) = default;
    dog(const dog &) = default;
    dog &operator=(dog &&) = default;
    dog &operator=(const dog &) = default;
    ~dog() = default;
private:
};
#endif

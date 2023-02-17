#include <iostream>
#include <string>
#include <utility>

class declaredmovable {
public:
    declaredmovable() = default;
    declaredmovable(const declaredmovable &) { std::cout << "declaredmovable: copied!"<< std::endl; }
    declaredmovable(declaredmovable &&) { std::cout << "declaredmovable: moved!"<< std::endl; }
};

class deletedmovable {
public:
    deletedmovable() = default;
    deletedmovable(const deletedmovable &) { std::cout << "declaredmovable: copied!"<<std::endl; }
    deletedmovable(deletedmovable &&) = delete;
};

class defaultmovable {
public:
    defaultmovable() = default;
    defaultmovable(const defaultmovable &) { std::cout << "defaultmovable: copied!" << std::endl; }
    defaultmovable(defaultmovable &&) = default;
};

class nodelcaremovable {
public:
    nodelcaremovable() = default;
    nodelcaremovable(const nodelcaremovable &) { std::cout << "nodelcaremovable : copied!" << std::endl; }
};

class hasnonmovable {
private:
    defaultmovable def{};
    declaredmovable dec{};
    nodelcaremovable nodec{};
    deletedmovable del{};
public:
    hasnonmovable() = default;
    /* hasnonmovable(hasnonmovable &&) = default; */
};

class onlyMovable {
private:
    defaultmovable def{};
    declaredmovable dec{};
    nodelcaremovable nedec{};
public:
    onlyMovable() = default;
    onlyMovable(onlyMovable &&) = default;
};

int main() {
    std::cout << "-----------------------------------\n";
    std::cout << "Test: Will move happen when move constructor is define : 1: to be default, 2:manually 3:not Declared\n";
    defaultmovable def{};
    defaultmovable mdef{std::move(def)};
    declaredmovable dec{};
    declaredmovable mdec{std::move(dec)};
    nodelcaremovable nodec{};
    nodelcaremovable mnodec{std::move(nodec)};

    std::cout << "-----------------------------------\n";
    std::cout << "Test:All member movable: Explicitly Declared Default | Manually Declared | No Declaration \n";
    onlyMovable o{};
    onlyMovable mo{std::move(o)};
    std::cout << "-----------------------------------\n";

    std::cout << "-----------------------------------\n";
    std::cout << "Test:Exist non-movable member: Explicitly Declared Default | Manually Declared | No Declaration | Explicitly deleted \n";
    hasnonmovable h{};
    hasnonmovable mh{std::move(h)};
    std::cout << "-----------------------------------\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Conclusion:\n";
    std::cout << "Declare move constructor(assignment) \"as default\" when posible, only then will the class be recognized as move constructable!\n";
    std::cout << "If member variable contain non-movable object, declare your own move special member function to move the movable part\n";

}

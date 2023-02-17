#include <vector>
#include <iostream>

// Test wether value copy for each loop will call copy constructor

class Entity {
public:
    Entity(int num):
        m_num(num) { 
            std::cout << "Construct Entity with " << num << std::endl;
        }
    Entity(const Entity& other){
        std::cout << "Entity Copy Constructor Called" << std::endl;
    }
private:
    int m_num;
};

int main() {
    std::cout << "Test : how dose vector init function works" << std::endl;
    std::vector<Entity> vec(10,0);

    std::cout << "Test : will normal for each loop copy object?" << std::endl;
    for(auto ent : vec) {
    }

    std::cout << "Test : will reference for each loop copy object?" << std::endl;
    for(auto& ent : vec) {
    }
}

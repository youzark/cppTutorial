#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/* const Qualifier:
 * 1. modify a variable qualified by const keyword "directly" is not allowed
 * 2. modify a variable qualified by const keyword "indirectly" will cause not defined behaviour
 * */

// const only serves as a promise that the parameter will not be changed,
// by can be bypassed by changing the memory directly
void setValue(const int &i) { *(int *)(&i) = 11; }

int globalJ = 10;
// const & guarantees the passed in reference "localJ" cannot be
void newValue(const int &localJ) {
  std::cout << "init value of localJ: " << localJ << std::endl;
  globalJ = 11;
  std::cout << "changed value of localJ: " << localJ << std::endl;
}

int main() {
  int i = 10;
  std::string v = "test";
  const int constParam = 10;
  setValue(i);
  setValue(constParam);
  newValue(globalJ);

  std::cout << "new value of constParam:" << constParam << std::endl;
  std::cout << "new value of i:" << i << std::endl;
}

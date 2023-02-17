#include <iostream>

class MsgFormatter {
public:
    MsgFormatter() {
        std::cout << "MsgFormatter constructor Called" << std::endl;
    }
};

class Logger {
public:
    const int LogLevel = 90; // declaration of LogLevel
    const static int constLogLevel; // Valid ,But alghouth constLogLevel is initialized here
                                         // This statement is still considered declaration
                                         // which means without proper definition 
                                         // we cannot access address of constLogLevel
    const static float constFloat; // Valid: declaration of constFloat
    const std::string  name = "youzark";
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};
const int Logger::constLogLevel = 10;


int main() {
    Logger logger{};
    Logger logger2{};
    std::cout << &logger.name << std::endl;
    std::cout << &logger2.name << std::endl;
    std::cout << logger.LogLevel << std::endl;
    std::cout << &logger.constLogLevel << std::endl;
    std::cout << &logger2.constLogLevel << std::endl;
    
    /* std::cout << Logger::constLogLevel << std::endl;  // Valid: output 10 */
    /* std::cout << &Logger::constLogLevel << std::endl; // ERROR: alghouth declaration provide initialization */
    /*                                                   // constLogLevel is not stored in Memory before definition */
    /* std::cout << &Logger::constLogLevel << std::endl; // Now ,it's valid to access address of constLogLevel */
}

#include <iostream>

class MsgFormatter {
public:
    MsgFormatter() {
        std::cout << "MsgFormatter constructor Called" << std::endl;
    }
};

class Logger {
public:
    static int LogLevel; // declaration of LogLevel
    const static int constLogLevel = 10; // Valid ,But alghouth constLogLevel is initialized here
                                         // This statement is still considered declaration
                                         // which means without proper definition 
                                         // we cannot access address of constLogLevel
    const static MsgFormatter formatter{}; // ERROR: non-integral static type must be 
                                           // initialized out of class definition
    const static MsgFormatter formatter; // Valid: declaration of formatter
    const static float constFloat = 1.0; // ERROR: same reason for formatter(non-integral)
    const static float constFloat; // Valid: declaration of constFloat
    Logger() {
        std::cout << "Log constructor Called" << std::endl;
    }
};

int Logger::LogLevel = 0; // definition of LogLevel
const int Logger::constLogLevel = 9; // Definition of constLogLevel
                                     // Note here: const quailfier is a must
const MsgFormatter Logger::formatter{}; // Definition of formatter
const float Logger::constFloat = 1.0; // Definition of constFloat

int main() {
    std::cout << Logger::constLogLevel << std::endl;  // Valid: output 10
    std::cout << &Logger::constLogLevel << std::endl; // ERROR: alghouth declaration provide initialization
                                                      // constLogLevel is not stored in Memory before definition
    std::cout << &Logger::constLogLevel << std::endl; // Now ,it's valid to access address of constLogLevel
}

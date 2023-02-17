#include <memory>
#include <iostream>
#include <fstream>
// base class : common features that all logger has
struct Logger {
	virtual void LogMessage(char const* message) = 0;
	virtual ~Logger() = default;
};

// derived class : special implementation with console
struct ConsoleLogger final : Logger {
	virtual void LogMessage(char const* message) override {
		std::cout << message << "\n";
	}
};

// derived class : special implementation with file
struct FileLogger final : Logger {
	FileLogger(char const* filename): m_output(filename) {}
	virtual void LogMessage(char const* message) override {
		m_output << message << "\n";
	}
private:
	std::ofstream m_output;
};

void runtimeDispatch() {
	auto logger{std::make_unique<ConsoleLogger>()};
	Logger* loggerPtr{logger.get()};
	loggerPtr->LogMessage("Hello World");
}

void LogHelloWorld(Logger& logger) {
	logger.LogMessage("Hello, World!");
}

int main() {
	auto cliLogger{ConsoleLogger{}};
	auto fileLogger{FileLogger{"logfile.txt"}};

	LogHelloWorld(cliLogger);
	LogHelloWorld(fileLogger);

}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>

// A simple interpreter for a custom language with string support
class Interpreter {
public:
    void execute(const std::string& program) {
        std::stringstream ss(program);
        std::string line;

        while (std::getline(ss, line)) {
            executeLine(line);
        }
    }

private:
    std::map<std::string, int> intVariables;    // Store integer variables
    std::map<std::string, std::string> strVariables; // Store string variables

    void executeLine(const std::string& line) {
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "set") {
            handleSetCommand(ss);
        } else if (command == "var") {
            handleVarCommand(ss);  // New handler for 'var'
        } else if (command == "print") {
            handlePrintCommand(ss);
        } else if (command == "add" || command == "sub" || command == "mul" || command == "div") {
            handleArithmeticCommand(command, ss);
        } else {
            std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        }
    }

    void handleSetCommand(std::stringstream& ss) {
        std::string varName;
        std::string value;
        ss >> varName;
        std::getline(ss, value);
        value = trim(value);

        // Check if the value is an integer or a string
        if (isInteger(value)) {
            intVariables[varName] = std::stoi(value);
        } else {
            strVariables[varName] = value;
        }
    }

    void handleVarCommand(std::stringstream& ss) {
        std::string varName;
        std::string value;
        ss >> varName >> value;
        value = trim(value);

        if (value.empty()) {
            std::cerr << "Error: No value provided for variable '" << varName << "'" << std::endl;
            return;
        }

        // Determine if the value is an integer or a string and store accordingly
        if (isInteger(value)) {
            intVariables[varName] = std::stoi(value);
            std::cout << "Integer variable '" << varName << "' initialized to " << intVariables[varName] << std::endl;
        } else {
            strVariables[varName] = value;
            std::cout << "String variable '" << varName << "' initialized to \"" << strVariables[varName] << "\"" << std::endl;
        }
    }

    void handlePrintCommand(std::stringstream& ss) {
        std::string varName;
        ss >> varName;

        if (intVariables.find(varName) != intVariables.end()) {
            std::cout << intVariables[varName] << std::endl;
        } else if (strVariables.find(varName) != strVariables.end()) {
            std::cout << strVariables[varName] << std::endl;
        } else {
            std::cerr << "Error: Variable '" << varName << "' not found!" << std::endl;
        }
    }

    void handleArithmeticCommand(const std::string& command, std::stringstream& ss) {
        std::string varName;
        int value;
        ss >> varName >> value;

        if (intVariables.find(varName) == intVariables.end()) {
            std::cerr << "Error: Variable '" << varName << "' not found!" << std::endl;
            return;
        }

        if (command == "add") {
            intVariables[varName] += value;
        } else if (command == "sub") {
            intVariables[varName] -= value;
        } else if (command == "mul") {
            intVariables[varName] *= value;
        } else if (command == "div") {
            if (value == 0) {
                std::cerr << "Error: Division by zero!" << std::endl;
            } else {
                intVariables[varName] /= value;
            }
        }
    }

    bool isInteger(const std::string& str) {
        try {
            std::stoi(str);  // Try to convert to integer
            return true;
        } catch (const std::invalid_argument&) {
            return false;
        }
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) return "";  // Empty string if no non-whitespace character
        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, (last - first + 1));
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No file provided. Usage: " << argv[0] << " <filename>.slp" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'!" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string program = buffer.str();

    // Create an interpreter instance and execute the program
    Interpreter interpreter;
    interpreter.execute(program);

    return 0;
}

#include "Logger.hpp"
#include <iostream>

void Logger::info(const char* msg) {
    std::cout << "INFO: " << msg << std::endl;
}
void Logger::error(const char* msg) {
    std::cerr << "ERROR: " << msg << std::endl;
}

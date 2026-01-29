#ifndef UTILS_HPP
#define UTILS_HPP
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Logger.hpp"

std::string toUpperWords(const std::string& str);
std::string toLowerWords(const std::string& str);
std::string trimSpacesComments(const std::string& s);
std::string cleanCharEnd(const std::string& v, char c);
bool        convertFileToLines(std::string file, std::vector<std::string>& lines);
bool        checkAllowedMethods(const std::string& m);
bool        parseKeyValue(const std::string& line, std::string& key, std::vector<std::string>& values);
bool        splitByChar(const std::string& line, std::string& key, std::string& value, char endChar);
size_t      convertMaxBodySize(const std::string& maxBody);
bool        isStringInVector(const std::string& toFind, const std::vector<std::string>& fromFind);
std::string normalizePath(const std::string& path);
bool        pathStartsWith(const std::string& path, const std::string& prefix);
template <typename type>
std::string typeToString(type _value) {
    std::stringstream ss;
    ss << _value;
    return ss.str();
}
template <typename type>
type stringToType(const std::string& str) {
    std::stringstream ss(str);
    type              value;
    ss >> value;
    return value;
}

// TODO : add time convert for  session timeout
#endif
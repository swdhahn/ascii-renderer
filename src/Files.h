#ifndef FILES
#define FILES

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

// reads from a file
std::string readFile(const char* path);

// reads from a file
std::vector<std::string> readFileIntoArray(const char* path);

// writes to a file
void writeFile(const char* path, const char* text);

// splits a string, i put it here b/c it doesn't really have anywhere to go, maybe move later?
std::vector<std::string> split(const std::string& str, const std::string& delim);

#endif // !FILES

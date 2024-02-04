#pragma once

#include "../globals.hpp"
#include "../types/.hpp"
#include "template.tpp"

namespace utils
{
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

// string
std::string &trim(std::string &str);
std::string lowerCase(const std::string str);
bool isNumber(const std::string str);
std::string hexToUtf8Char(const std::string &hex);
std::string getlineCustom(std::istringstream &requestTextStream);
std::vector<std::string> tokenize(const std::string &original, const char &delim);
std::string removeCharacter(const std::string str, const char charToRemove);

// fd_set
fd_set fdSetFrom(const Sockets sds);
Sockets sdsIn(const fd_set fdSet, const Sockets sds);

// file
FileContentResult fileContent(const std::string &path);
HttpResponse writeToFile(const std::string &path, const std::string &fileContent);
std::string contentType(const std::string &path);
IsDirectoryResult isDirectory(const std::string &path);
bool createFile(const std::string &fileName);

// vector
std::string join(const std::vector<std::string> &list, const std::string &delimiter);
} // namespace utils

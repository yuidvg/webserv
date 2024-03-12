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
std::vector<std::string> split(const std::string &original, const std::string &delim);
std::string removeCharacter(const std::string str, const char charToRemove);
StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);
ReadFileResult readFile(const int fd);
std::string itoa(const int &num);

// uri
bool isAbsolutePath(const std::string &path);
bool isAbsoluteUri(const std::string &uri);

// file
FileContentResult fileContent(const std::string &path);
HttpResponse writeToFile(const std::string &path, const std::string &fileContent);
std::string contentType(const std::string &path);
IsDirectoryResult isDirectory(const std::string &path);
bool createFile(const std::string &fileName, const std::string &path);

// vector
std::string join(const std::vector<std::string> &list, const std::string &delimiter);

// status
bool isStatusInRange(const int status);

// cstrArray
void deleteCStrArray(char *const *array);

// kernelEvent
bool registerEvent(const uintptr_t identifier, const int16_t filter);

} // namespace utils

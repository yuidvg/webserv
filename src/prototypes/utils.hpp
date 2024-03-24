#pragma once

#include "../types/bottom.hpp"
#include "utilTemplates.tpp"

namespace utils
{
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

ErrorPage generateErrorPage(const int statusCode, const std::string &path);
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
bool isHex(const std::string &str);

// uri
bool isAbsolutePath(const std::string &path);
bool isAbsoluteUri(const std::string &uri);

// file
FileContentResult fileContent(const std::string &path);
std::string contentType(const std::string &path);
bool isDirectory(const std::string &path);
bool createFile(const std::string &fileName, const std::string &path);
bool writeToFile(const std::string &path, const std::string &content);

// vector
std::string join(const std::vector<std::string> &list, const std::string &delimiter);

// status
bool isStatusInRange(const int status);

// cstrArray
void deleteCStrArray(char *const *array);

// kernelEvent
bool registerEvent(const uintptr_t identifier, const int16_t filter);
bool setEventFlags(const uintptr_t identifier, const int16_t filter, const uint16_t flags);

// socket
SocketsResult createListenSockets(const Servers servers);
ConnectedInternetSocketResult newConnectedInternetSocket(const Socket &listenSocket);

} // namespace utils

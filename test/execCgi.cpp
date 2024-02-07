
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

char *const *mapStringStringToCStringArray(const std::map<std::string, std::string> &envMap)
{
    char **envArray = new char *[envMap.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it)
    {
        std::string envString = it->first + "=" + it->second;
        envArray[i] = new char[envString.size() + 1];
        std::copy(envString.begin(), envString.end(), envArray[i]);
        envArray[i][envString.size()] = '\0'; // Null-terminate the string
        ++i;
    }
    envArray[envMap.size()] = NULL; // Last element is NULL for execve
    return envArray;
}

int main()
{
    std::map<std::string, std::string> env;

    env["AUTH_TYPE"] = "basic";
    env["CONTENT_LENGTH"] = "32";
    char *argv[2];
    argv[0] = "./www/dump-param.cgi";
    argv[1] = NULL;
    errno = 0;
    execve("./www/dump-param.cgi", argv, mapStringStringToCStringArray(env));
    perror("execve failed");
    // Check if the response is successful
    return 0;
}

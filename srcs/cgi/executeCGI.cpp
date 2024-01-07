#include "../consts.hpp"
#include "../map/value.cpp"
#include "determineCGI.cpp"

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

char *const *enviromentVariables(const ParsedRequest request, const Server server)
{
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = map::value(request.header, std::string("Authorization"));
    env["CONTENT_LENGTH"] = map::value(request.header, std::string("Content-Length"));
    env["CONTENT_TYPE"] = map::value(request.header, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = request.uri;
    env["PATH_TRANSLATED"] = request.uri;
    // To be implemented
    // env["QUERY_STRING"] = ;
    // env["REMOTE_ADDR"] = ;
    // env["REMOTE_HOST"] = ;
    // env["REMOTE_IDENT"] = ;
    // env["REMOTE_USER"] = ;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = map::value(request.header, request.uri);
    env["SERVER_NAME"] = server.server_name;
    env["SERVER_PORT"] = server.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    return mapStringStringToCStringArray(env);
}

const std::string executeCGI(const ParsedRequest request, const Server server, const int clientSocket)
{
    const pid_t pid = fork();
    if (pid == -1)
        return false;
    else if (pid == 0) // child process
    {
        execve(request.uri.c_str(), NULL, enviromentVariables(request, server));
        std::cerr << "execve failed" << std::endl;
        write(STDOUT_FILENO, "Status: 500\n\n", 13);
    }
    else // parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return ;
        else
            return ;
    }
    return cgiResponse;
}

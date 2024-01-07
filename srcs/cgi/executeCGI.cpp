#include "../consts.hpp"
#include "../map/value.cpp"
#include "determineCGI.cpp"

const char **enviromentVariables(const ParsedRequest request, const Server server)
{
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = map::value(request.header, std::string("Authorization"));
    env["CONTENT_LENGTH"] = map::value(request.header, std::string("Content-Length"));
    env["CONTENT_TYPE"] = map::value(request.header, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = request.uri;
    env["PATH_TRANSLATED"] = ;
    env["QUERY_STRING"] = ;
    env["REMOTE_ADDR"] = ;
    env["REMOTE_HOST"] = ;
    env["REMOTE_IDENT"] = ;
    env["REMOTE_USER"] = ;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = map::value(request.header, request.uri);
    env["SERVER_NAME"] = server.server_name;
    env["SERVER_PORT"] = server.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
}

const bool executeCGI(const ParsedRequest request, const Route server, const int clientSocket)
{
    const pid_t pid = fork();
    if (pid == -1)
        return false;
    else if (pid == 0) // child process
    {
        execve(request.uri.c_str(), NULL, enviromentVariables(request, server));
    }
    else // parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return true;
        else
            return false;
    }
}

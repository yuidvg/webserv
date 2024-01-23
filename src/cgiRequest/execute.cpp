#include ".hpp"

namespace cgi
{

char *const *mapStringStringToCStringArray(const std::map<std::string, std::string> envMap)
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

char *const *enviromentVariables(const HttpRequest request, const Server server)
{
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = utils::value(request.headers, std::string("Authorization"));
    env["CONTENT_LENGTH"] = utils::value(request.headers, std::string("Content-Length"));
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = request.target;
    env["PATH_TRANSLATED"] = request.target;
    // To be implemented
    env["QUERY_STRING"] = request.target.substr(request.target.find("?") + 1);
    // env["REMOTE_ADDR"] = ;
    // env["REMOTE_HOST"] = ;
    // env["REMOTE_IDENT"] = ;
    // env["REMOTE_USER"] = ;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = utils::value(request.headers, request.target);
    env["SERVER_NAME"] = server.name;
    env["SERVER_PORT"] = server.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    return mapStringStringToCStringArray(env);
}

ResponseResult execute(const HttpRequest request, const Server server)
{
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        std::cerr << "pipe failed" << std::endl;
        return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    }
    const pid_t pid = fork();
    if (pid == -1)
        return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    else if (pid == 0) // child process
    {
        close(pipefds[IN]);
        execve(request.target.c_str(), NULL, enviromentVariables(request, server));
        std::cerr << "execve failed" << std::endl;
    }
    else // parent process
    {
        close(pipefds[OUT]);
        write(pipefds[IN], request.body.c_str(), request.body.size());
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return ResponseResult::Error(SERVER_ERROR_RESPONSE);
        else
            return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    }
    return ResponseResult::Success(SUCCESS_RESPONSE);
}

} // namespace cgi
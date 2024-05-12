#include "../all.hpp"

namespace
{
char *const *mapStringStringToCStringArray(const StringMap &envMap)
{
    char **envArray = new char *[envMap.size() + 1];
    int i = 0;
    for (StringMap::const_iterator it = envMap.begin(); it != envMap.end(); ++it)
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

std::string authType(const HttpRequest &request)
{
    const std::string authorization = utils::value(request.headers, std::string("authorization"));
    const std::vector< std::string > tokens = utils::split(authorization, " ");
    return tokens.size() > 0 ? tokens[0] : "";
}

StringMap getCgiEnvs(const HttpRequest &httpRequest)
{
    const Uri uri = segment(httpRequest);
    StringMap env;
    env.insert(std::make_pair("AUTH_TYPE", authType(httpRequest)));
    env.insert(std::make_pair("CONTENT_LENGTH", utils::itoa(httpRequest.body.size())));
    env.insert(std::make_pair("CONTENT_TYPE", utils::value(httpRequest.headers, std::string("content-type"))));
    env.insert(std::make_pair("GATEWAY_INTERFACE", GATEWAY_INTERFACE));
    env.insert(std::make_pair("PATH_INFO", uri.extraPath));
    env.insert(std::make_pair(
        "PATH_TRANSLATED",
        uri.extraPath.size() > 0
            ? utils::resolvePath(
                  uri.extraPath,
                  CONFIG.getServer(httpRequest.host, httpRequest.socket.serverPort).getLocation(httpRequest.target))
            : ""));
    env.insert(std::make_pair("QUERY_STRING", uri.queryString));
    env.insert(std::make_pair("REMOTE_ADDR", httpRequest.socket.clientIp));
    env.insert(std::make_pair("REQUEST_METHOD", httpRequest.method));
    env.insert(std::make_pair("SCRIPT_NAME", uri.scriptPath));
    env.insert(std::make_pair("SERVER_NAME", httpRequest.host));
    env.insert(std::make_pair("SERVER_PORT", std::to_string(httpRequest.socket.serverPort)));
    env.insert(std::make_pair("SERVER_PROTOCOL", SERVER_PROTOCOL));
    env.insert(std::make_pair("SERVER_SOFTWARE", SERVER_SOFTWARE));
    return env;
}

} // namespace

Option< Socket > createCgiProcess(const HttpRequest &httpRequest, const std::string &scriptPath)
{
    int socketPair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) == 0 &&
        utils::registerEvent(socketPair[SERVER_END], EVFILT_READ))
    {
        std::cout << "socketpair succeeded" << std::endl;
        const pid_t pid = fork();
        if (pid == -1)
        {
            close(socketPair[SERVER_END]);
            close(socketPair[CGI_END]);
            return Option< Socket >();
        }
        else if (pid == 0) // cgi process
        {
            const std::string tmpFileName = "/tmp/cgi_input_" + std::to_string(getpid());
            std::ofstream tmpFile(tmpFileName.c_str());
            if (!tmpFile.is_open())
            {
                std::cerr << "Failed to open temporary file for CGI input" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            tmpFile << httpRequest.body;
            tmpFile.close();
            int tmpFileDescriptor = open(tmpFileName.c_str(), O_RDONLY);
            if (tmpFileDescriptor == -1)
            {
                std::cerr << "Failed to open temporary file descriptor for CGI input" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            std::cout << "child process" << std::endl;
            if (close(socketPair[SERVER_END]) == 0 && dup2(tmpFileDescriptor, STDIN_FILENO) != -1 &&
                dup2(socketPair[CGI_END], STDOUT_FILENO) != -1)
            {
                StringMap cgiEnvs = getCgiEnvs(httpRequest);
                errno = 0;
                char *const *envp = mapStringStringToCStringArray(cgiEnvs);
                char *args[2];
                args[0] = const_cast< char * >(scriptPath.c_str());
                args[1] = NULL;
                execve(scriptPath.c_str(), args, envp);
                std::cerr << "execve failed: " << strerror(errno) << std::endl;
                utils::deleteCStrArray(envp);
            }
            close(socketPair[CGI_END]);
            std::exit(EXIT_FAILURE);
        }
        else // server process
        {
            close(socketPair[CGI_END]);
            return Option< Socket >(Socket(socketPair[SERVER_END], CGI, 0, 0, "", pid));
        }
    }
    else
    {
        std::cerr << "socketpair/registerEvent failed" << std::endl;
        return Option< Socket >();
    }
}

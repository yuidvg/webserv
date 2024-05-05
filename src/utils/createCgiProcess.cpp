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

} // namespace

Option<Socket> createCgiProcess(const StringMap &envs, const std::string &scriptPath)
{
    int socketPair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) == 0 &&
        utils::registerEvent(socketPair[SERVER_END], EVFILT_WRITE) &&
        utils::registerEvent(socketPair[SERVER_END], EVFILT_READ) &&
        utils::setEventFlags(socketPair[SERVER_END], EVFILT_WRITE, EV_DISABLE))
    {
        std::cout << "socketpair succeeded" << std::endl;
        const pid_t pid = fork();
        if (pid == -1)
        {
            close(socketPair[SERVER_END]);
            close(socketPair[CGI_END]);
            return Option<Socket>();
        }
        else if (pid == 0) // cgi process
        {
            std::cout << "child process" << std::endl;
            if (close(socketPair[SERVER_END]) == 0 && dup2(socketPair[CGI_END], STDIN_FILENO) != -1 &&
                dup2(socketPair[CGI_END], STDOUT_FILENO) != -1)
            {
                errno = 0;
                char *const *envp = mapStringStringToCStringArray(envs);
                char *args[2];
                args[0] = const_cast<char *>(scriptPath.c_str());
                args[1] = NULL;
                execve(scriptPath.c_str(), args, envp);
                std::cerr << "execve failed: " << strerror(errno) << std::endl;
                utils::deleteCStrArray(envp);
            }
            std::exit(EXIT_FAILURE);
        }
        else // server process
        {
            close(socketPair[CGI_END]);
            return Option<Socket>(Socket(socketPair[SERVER_END], CGI, 0, 0, "", pid));
        }
    }
    else
    {
        std::cerr << "socketpair/registerEvent failed" << std::endl;
        return Option<Socket>();
    }
}

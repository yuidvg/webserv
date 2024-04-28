#include "../all.hpp"

std::pair<EventDatas, Strings> retrieveDatas(const KEvents &readEvents)
{
    EventDatas eventDatas;
    Strings errors;
    for (KEvents::const_iterator it = readEvents.begin(); it != readEvents.end(); ++it)
    {
        const struct kevent &event = *it;
        const int sd = event.ident;
        const intptr_t size = event.data;
        char *buffer = new char[size + 1]; // +1 for null terminator
        const ssize_t receivedLength = recv(sd, buffer, size, 0);
        if (receivedLength > 0)
        {
            buffer[receivedLength] = '\0';
            const std::string data(buffer);
            eventDatas.push_back(EventData(Socket(sd, 0, 0, "", 0), data));
        }
        else if (receivedLength == 0)
        {
            std::cerr << "recv() failed: connection closed by peer" << std::endl;
            close(sd);
        }
        else
        {
            std::cerr << "recv() failed: " << std::endl;
            close(sd);
        }
        delete[] buffer;
    }
    return std::make_pair(eventDatas, Strings());
}

#include ".hpp"

namespace
{
Socket maxSdSocket(const Sockets &sockets)
{
    unsigned int maxIndex = 0;
    for (unsigned int i = 0; i < sockets.size(); i++)
        if (sockets[i].descriptor > sockets[maxIndex].descriptor)
            maxIndex = i;
    return sockets[maxIndex];
}
} // namespace

ReadableWritableSocketsResult readableOrWritableSockets(const Sockets sockets)
{
    int kq = kqueue();
    if (kq == -1)
    {
        return ReadableWritableSocketsResult::Error("kqueue() failed: " + std::string(strerror(errno)));
    }

    std::vector<struct kevent> events; // イベント構造体の準備

    // 各socketに対してイベントを登録する
    for (Sockets::iterator socketIt = sockets.begin(); socketIt != sockets.end(); ++socketIt)
    {
        struct kevent event;
        EV_SET(&event, socketIt->descriptor, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, &socket);
        events.push_back(event);
    }

    // kqueueへのイベント登録
    if (kevent(kq, events.data(), events.size(), nullptr, 0, nullptr) < 0)
    {
        return ReadableWritableSocketsResult::Error("kevent() failed: " + std::string(strerror(errno)));
    }

    // 発生したイベントの処理
    struct kevent triggeredEvents[events.size()];
    int numEvents;

    numEvents = kevent(kq, nullptr, 0, triggeredEvents, events.size(), nullptr);
    if (numEvents < 0)
    {
        return ReadableWritableSocketsResult::Error("kevent() failed: " + std::string(strerror(errno)));
    }
    else if (numEvents == 0)
    {
        continue; // タイムアウトした場合など
    }

    return ReadableWritableSocketsResult::Success(utils::socketsIn(kq, sockets));
}

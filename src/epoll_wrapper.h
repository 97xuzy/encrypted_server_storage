#pragma once
#include <vector>

class EventPoll
{
private:
    int epfd;
    struct epoll_event *events;
public:
    EventPoll();
    ~EventPoll();
    void add_fd(int fd);
    std::vector<int> wait_events();
};


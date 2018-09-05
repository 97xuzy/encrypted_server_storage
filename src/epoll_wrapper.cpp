#ifdef __unix__

#include "epoll_wrapper.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdexcept>
using std::runtime_error;
#include <vector>
using std::vector;


EventPoll::EventPoll()
{
    this->epfd = epoll_create(1000);
    this->events = new epoll_event[1000];
}

EventPoll::~EventPoll()
{
    close(epfd);
}

void EventPoll::add_fd(int fd)
{
    static struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = fd;
    int res = epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &ev);
    if(res == -1)
    {
        throw new runtime_error("fail to add fd in epoll");
    }
}

vector<int> EventPoll::wait_events()
{
    int nfds = epoll_wait(this->epfd, events, 1000, 5);

    if (nfds < 0)
    {
        throw new runtime_error("Error in epoll_wait()");
    }
  
    vector<int> fd_with_events;
    // for each ready socket
    for(int i = 0; i < nfds; i++) {
      int fd = events[i].data.fd;
      fd_with_events.push_back(fd);
    }
    return fd_with_events;
}


#endif

#include "Downlink_channel.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>

Downlink_channel::Downlink_channel(int port) : max_number_of_events(255)
{
    this->port = port;
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(this->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("ERROR: ");
        throw std::string("Bind fail");
    }

    set_socket_non_blocking(this->socket_fd);

    if((this->epoll_fd = epoll_create(max_number_of_events)) == -1)
    {
        perror("ERROR: ");
        throw std::string("Bind fail");
    }

    std::memset(&(this->event), 0, sizeof(struct epoll_event));
    this->event.data.fd = this->socket_fd;
    this->event.events = EPOLLIN | EPOLLET;

    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->socket_fd, &event) == -1)
    {
        perror("ERROR: ");
        throw std::string("Epoll ctl fail");
    }

    this->events = new epoll_event[max_number_of_events];
    std::memset(this->events, 0, sizeof(struct epoll_event) * max_number_of_events);
}

Downlink_channel::~Downlink_channel()
{
    close(this->socket_fd);
}

void Downlink_channel::set_socket_non_blocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

void Downlink_channel::handle_connections()
{
    int number_of_incoming_events;
    number_of_incoming_events = epoll_wait(this->epoll_fd, this->events, max_number_of_events, 0);

    if(number_of_incoming_events != 0)
        std::cout << number_of_incoming_events << std::endl;

    for(int i = 0; i < number_of_incoming_events; i++)
    {
        read_incoming_data(this->events[i].data.fd);
    }
}

void Downlink_channel::read_incoming_data(int event_fd)
{
    ssize_t bytes_count = receive_message(event_fd);

    if(bytes_count == -1 && errno != EAGAIN)
    {
        perror("ERROR: ");
        throw std::string("Read fail");
    }
}
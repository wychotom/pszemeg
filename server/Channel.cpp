#include "Channel.h"
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
#include <map>

Channel::Channel(int port, size_t counter_reset) : max_number_of_events(255), counter(1)
{
    this->port = port;
    this->counter_reset = counter_reset;
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("ERROR: ");
        throw std::string("Bind fail");
    }

    set_socket_non_blocking(this->socket_fd);

    if(listen(this->socket_fd, 5) == -1)
    {
        perror("ERROR: ");
        throw std::string("Listen fail");
    }

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

Channel::~Channel()
{
    delete[] this->events;
    close(this->socket_fd);
}

void Channel::set_socket_non_blocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

void Channel::handle_connections()
{
    int number_of_incoming_events;

    if(++counter >= this->counter_reset)
    {
        counter = 0;
    }

    number_of_incoming_events = epoll_wait(this->epoll_fd, this->events, max_number_of_events, 0);

    for(int i = 0; i < number_of_incoming_events; i++)
    {
        if(this->events[i].events & EPOLLIN)
        {
            if (this->socket_fd == this->events[i].data.fd)
            {
                accept_new_connection();
            }
            else
            {
                read_incoming_data(this->events[i].data.fd);
            }
        }
        else
        {
            clients_fds.erase(this->events[i].events);
            close(this->events[i].events);
        }
    }
}

void Channel::accept_new_connection()
{
    socklen_t client_socket_length = sizeof(client);
    int new_client_fd;

    while((new_client_fd = accept(this->socket_fd, &client, &client_socket_length)) != -1)
    {
        socklen_t addrlen;
        char hostname_buffer[NI_MAXHOST], service_buffer[NI_MAXSERV];

        if (getnameinfo(&client, client_socket_length, 
                        hostname_buffer, sizeof(hostname_buffer), 
                        service_buffer, sizeof(service_buffer), 
                        NI_NUMERICHOST | NI_NUMERICSERV) == 0)
        {
            std::cout << "Hostname: [" << hostname_buffer << "] Service: [" << service_buffer << "]" << std::endl;
        }

        set_socket_non_blocking(new_client_fd);

        clients_fds.insert(std::pair<int,int>(new_client_fd, clients_fds.size()));
        send_data(new_client_fd, "HELLO");

        event.data.fd = new_client_fd;
        event.events = EPOLLIN | EPOLLET;

        if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, new_client_fd, &event) == -1)
        {
            perror("ERROR: ");
            throw std::string("Epoll ctl fail");
        }

    }
}

void Channel::read_incoming_data(int event_fd)
{
    ssize_t bytes_count = recv_message(event_fd);

    if(bytes_count == -1 && errno != EAGAIN)
    {
        perror("ERROR: ");
        throw std::string("Read fail");
    }
    else if(bytes_count == 0)
    {
        std::cout << event_fd << " has disconnected" << std::endl;
        clients_fds.erase(event_fd);
        close(event_fd);
    }
}

void Channel::send_data(int socket_fd, const char message[1024])
{
    if(send(socket_fd, message, strlen(message) + 1, 0) < 0)
    {
        perror("ERROR: ");
        throw std::string("Send fail");
    }
}

size_t Channel::getCounter()
{
    return counter;
}

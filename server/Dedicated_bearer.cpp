#include "Dedicated_bearer.h"
#include "UE.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <vector>

Dedicated_bearer::Dedicated_bearer(int port, std::vector<UE*> &clients) : max_number_of_events(255), clients(clients)
{
    this->port = port;
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(static_cast<uint16_t>(this->port));
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
        throw std::string("Epoll_ctl fail");
    }

    this->events = new epoll_event[max_number_of_events];
    std::memset(this->events, 0, sizeof(struct epoll_event) * max_number_of_events);
}

Dedicated_bearer::~Dedicated_bearer()
{
    for(auto client : clients)
    {
        close(client->get_socket_fd());
    }

    delete[] this->events;
    close(this->socket_fd);
}

void Dedicated_bearer::set_socket_non_blocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

void Dedicated_bearer::handle_connections()
{
    int number_of_incoming_events;

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
            close(this->events[i].events);
        }
    }
}

void Dedicated_bearer::accept_new_connection()
{
    socklen_t client_socket_length = sizeof(client);
    int new_client_fd;

    while((new_client_fd = accept(this->socket_fd, &client, &client_socket_length)) != -1)
    {
        set_socket_non_blocking(new_client_fd);

        event.data.fd = new_client_fd;
        event.events = EPOLLIN | EPOLLET;

        if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, new_client_fd, &event) == -1)
        {
            perror("ERROR: ");
            throw std::string("Epoll_ctl fail");
        }
    }
}

void Dedicated_bearer::read_incoming_data(int event_fd)
{
    ssize_t bytes_count = recv_message(event_fd);

    if(bytes_count == -1 && errno != EAGAIN)
    {
        perror("ERROR: ");
        throw std::string("Read fail");
    }
    else if(bytes_count == 0)
    {
        auto first_client_occurrence_iterator = std::find_if(clients.begin(), clients.end(), [this, &event_fd](UE* client) {
            return client->get_socket_fd() == event_fd;
        });

        if (first_client_occurrence_iterator != clients.end())
        {
            delete *first_client_occurrence_iterator;
            clients.erase(first_client_occurrence_iterator);
            close(event_fd);
        }
    }
}

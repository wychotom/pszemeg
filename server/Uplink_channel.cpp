#include "Uplink_channel.h"

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

Uplink_channel::Uplink_channel(int port, size_t counter_reset) : counter(1)
{
    this->port = port;
    this->counter_reset = counter_reset;

    int broadcast = 1;

    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(this->port);
    this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* socket creation */
    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->socket_fd < 0)
    {
        perror("ERROR: ");
        throw std::string("socket fail");
    }

    if (setsockopt(this->socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1)
    {
        perror("ERROR: ");
        throw std::string("setsockopt fail");
    }

    /* bind any port */
    this->client_addr.sin_family = AF_INET;
    this->client_addr.sin_port = htons(0);
    this->client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(this->socket_fd, (struct sockaddr*) &client_addr, sizeof(this->client_addr)) < 0)
    {
        perror("ERROR: ");
        throw std::string("bind fail");
    }
}

void Uplink_channel::send_message(void *message, size_t size)
{
    if(sendto(this->socket_fd, message, size, 0, (struct sockaddr*) &this->server_addr, sizeof(this->server_addr)) < 0)
    {
        close(this->socket_fd);
        perror("ERROR: ");
        throw std::string("sendto fail");
    }
}

size_t Uplink_channel::get_counter()
{
    if(++this->counter >= this->counter_reset)
    {
        this->counter = 0;
    }

    return this->counter;
}

Uplink_channel::~Uplink_channel()
{
    close(this->socket_fd);
}

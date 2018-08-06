#include "Downlink_channel.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <fcntl.h>

Downlink_channel::Downlink_channel(int port)
{
    this->port = port;

    this->socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(this->socket_fd < 0)
    {
        perror("ERROR: ");
        throw std::string("downlink socket fail");
    }

    set_non_blocking(this->socket_fd);

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(this->socket_fd, (struct sockaddr*) &addr, sizeof(this->addr)) < 0)
    {
        perror("ERROR: ");
        throw std::string("bind fail");
    }
}

Downlink_channel::~Downlink_channel()
{
    close(this->socket_fd);
}

void Downlink_channel::set_non_blocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}
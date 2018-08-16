#include "Downlink_channel.h"
#include "Log.h"
#include "Antenna.h"

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

Downlink_channel::Downlink_channel(int port)
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
}

Downlink_channel::~Downlink_channel()
{
    close(this->socket_fd);
}

void Downlink_channel::handle_connections()
{
    read_incoming_data(this->socket_fd);
}

void Downlink_channel::read_incoming_data(int socket_fd)
{
    if(receive_message(socket_fd) > 0)
    {
        this->antenna.set_transmitting(Colors::Blue);
    }
}
#include "PBCH.h"
#include "channels_struct.h"

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

PBCH::PBCH(int port)
{
    this->port = port;

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

void PBCH::send_MIB()
{
    struct MIB_MESSAGE new_message;

    new_message.broadcast_port = PORTS::broadcast_port;
    new_message.dl_sch_port = PORTS::dl_sch_port;
    new_message.ul_sch_port = PORTS::ul_sch_port;
    new_message.pdcch_port = PORTS::pdcch_port;
    new_message.pucch_port = PORTS::pucch_port;
    new_message.prach_port = PORTS::prach_port;

    if(sendto(this->socket_fd, &new_message, sizeof(struct MIB_MESSAGE), 0, (struct sockaddr*) &server_addr, sizeof(this->server_addr)) < 0)
    {
        close(this->socket_fd);
        perror("ERROR: ");
        throw std::string("sendto fail");
    }

}

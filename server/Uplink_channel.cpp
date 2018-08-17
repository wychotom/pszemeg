#include "Uplink_channel.h"

#include "Log.h"

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
#include <arpa/inet.h>
#include <ctime>

Uplink_channel::Uplink_channel(int port, double send_frequency) : last_event_time(clock())
{
    this->port = port;
    this->send_frequency = send_frequency;

    int broadcast = 1;

    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(static_cast<uint16_t>(this->port));
    this->server_addr.sin_addr.s_addr = htons(INADDR_ANY);

    #ifdef LANCONN
        this->server_addr.sin_addr.s_addr = inet_addr("192.168.40.255");
    #endif

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
    this->antenna.set_transmitting(Colors::Yellow);

    if(sendto(this->socket_fd, message, size, 0, (struct sockaddr*) &this->server_addr, sizeof(this->server_addr)) < 0)
    {
        close(this->socket_fd);
        perror("ERROR: ");
        throw std::string("sendto fail");
    }
}

void Uplink_channel::run_timer_job()
{
    clock_t now = clock();
    double elapsed_secs = double(now - this->last_event_time) / CLOCKS_PER_SEC;

    if(elapsed_secs >= this->send_frequency)
    {
        this->last_event_time = now;
        timer_job();
    }
}

Uplink_channel::~Uplink_channel()
{
    close(this->socket_fd);
}

void Uplink_channel::timer_job()
{
}

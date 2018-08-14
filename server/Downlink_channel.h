#ifndef SERVER_DOWNLINK_CHANNEL_H
#define SERVER_DOWNLINK_CHANNEL_H

#include "Antenna.h"

#include <netinet/in.h>
#include <sys/epoll.h>

class Downlink_channel
{
    public:
        Downlink_channel(int port);
        ~Downlink_channel();
        void handle_connections();
        virtual ssize_t receive_message(int socket_fd) = 0;

    private:
        void read_incoming_data(int socket_fd);

        int port;
        int socket_fd;
        struct sockaddr_in addr;
        struct sockaddr client;

        Antenna antenna;
};


#endif //SERVER_DOWNLINK_CHANNEL_H

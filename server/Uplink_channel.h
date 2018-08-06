#ifndef SERVER_UPLINK_CHANNEL_H
#define SERVER_UPLINK_CHANNEL_H

#include <netinet/in.h>
#include <sys/epoll.h>

class Uplink_channel
{
    public:
        Uplink_channel(int port);
        void send_message(void *message, size_t size);

    private:
        int port;
        int socket_fd;
        struct sockaddr_in server_addr, client_addr;
};


#endif //SERVER_UPLINK_CHANNEL_H

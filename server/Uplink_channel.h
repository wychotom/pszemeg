#ifndef SERVER_UPLINK_CHANNEL_H
#define SERVER_UPLINK_CHANNEL_H

#include <netinet/in.h>
#include <sys/epoll.h>

class Uplink_channel
{
    public:
        Uplink_channel(int port, size_t counter_reset);
        void send_message(void *message, size_t size);
        size_t get_counter();

    private:
        int port;
        int socket_fd;
        size_t counter;
        size_t counter_reset;
        struct sockaddr_in server_addr, client_addr;
};


#endif //SERVER_UPLINK_CHANNEL_H

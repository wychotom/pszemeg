#ifndef SERVER_DOWNLINK_CHANNEL_H
#define SERVER_DOWNLINK_CHANNEL_H

#include <netinet/in.h>

class Downlink_channel
{
    public:
        Downlink_channel(int port);
        ~Downlink_channel();
        virtual void receive_message() = 0;

    private:
        int port;
        struct sockaddr_in addr;
        void set_non_blocking(int socket_fd);

    protected:
        int socket_fd;
};


#endif //SERVER_DOWNLINK_CHANNEL_H

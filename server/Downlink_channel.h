#ifndef SERVER_DOWNLINK_CHANNEL_H
#define SERVER_DOWNLINK_CHANNEL_H

#include <netinet/in.h>
#include <sys/epoll.h>

class Downlink_channel
{
    public:
        Downlink_channel(int port);
        ~Downlink_channel();
        void handle_connections();
        virtual ssize_t receive_message(int event_fd) = 0;

    private:
        void set_socket_non_blocking(int socket_fd);
        void accept_new_connection();
        void read_incoming_data(int event_fd);

        int port;
        int socket_fd, epoll_fd;
        struct sockaddr_in addr;
        struct sockaddr client;
        struct epoll_event event;
        struct epoll_event *events;
        const int max_number_of_events;
};


#endif //SERVER_DOWNLINK_CHANNEL_H

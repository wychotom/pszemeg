#ifndef CHANNEL
#define CHANNEL

#include <netinet/in.h>
#include <sys/epoll.h>

class Channel
{
    public:
        Channel(int port);
        void handle_connections();

    private:
        void set_socket_non_blocking(int socket_fd);
        void accept_new_connection();

        int port;
        int socket_fd, epoll_fd;
        struct sockaddr_in addr;
        struct sockaddr client;
        struct epoll_event event;
        struct epoll_event *events;

        const int max_number_of_events;
};

#endif
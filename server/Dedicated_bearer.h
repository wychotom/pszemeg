#ifndef SERVER_DEDICATED_BEARER_H
#define SERVER_DEDICATED_BEARER_H

#include "UE.h"

#include <netinet/in.h>
#include <sys/epoll.h>
#include <vector>

class Dedicated_bearer
{
    public:
        Dedicated_bearer(int port, std::vector<UE*> &clients);
        ~Dedicated_bearer();
        void handle_connections();

    private:
        void set_socket_non_blocking(int socket_fd);
        void accept_new_connection();
        void read_incoming_data(int event_fd);
        virtual ssize_t recv_message(int event_fd) = 0;

        int port;
        int socket_fd, epoll_fd;
        struct sockaddr_in addr;
        struct sockaddr client;
        struct epoll_event event;
        struct epoll_event *events;
        const int max_number_of_events;

        std::vector<UE*> &clients;
};


#endif //SERVER_DEDICATED_BEARER_H

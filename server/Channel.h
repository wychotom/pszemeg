#ifndef _CHANNEL
#define _CHANNEL

#include "UE.h"
#include <netinet/in.h>
#include <sys/epoll.h>
#include <vector>

class Channel
{
    public:
        Channel(int port, size_t counter_reset);
        ~Channel();
        void handle_connections();
        size_t getCounter();
        static std::vector<UE> clients;

    private:
        void set_socket_non_blocking(int socket_fd);
        void accept_new_connection();
        void read_incoming_data(int event_fd);
        void send_data(int socket_fd, const char message[1024]);
        virtual void send_message(int socket_fd) = 0;
        virtual int recv_message(int event_fd) = 0;

        int port;
        int socket_fd, epoll_fd;
        struct sockaddr_in addr;
        struct sockaddr client;
        struct epoll_event event;
        struct epoll_event *events;
        size_t counter;
        const int max_number_of_events;

    protected:
        size_t counter_reset;
};

#endif
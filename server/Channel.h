#ifndef CHANNEL
#define CHANNEL

#include <netinet/in.h>
#include <sys/epoll.h>
#include <map>

class Channel
{
    public:
        Channel(int port);
        ~Channel();
        void handle_connections();

    protected:
        virtual void send_message(int socket_fd) = 0;

    private:
        void set_socket_non_blocking(int socket_fd);
        void accept_new_connection();
        void read_incoming_data(int event_fd);
        void send_data(int socket_fd, const char message[1024]);

        int port;
        int socket_fd, epoll_fd;
        struct sockaddr_in addr;
        struct sockaddr client;
        struct epoll_event event;
        struct epoll_event *events;

        const int max_number_of_events;

        std::map<int, int> clients_fds;
};

#endif
#ifndef SERVER_UPLINK_CHANNEL_H
#define SERVER_UPLINK_CHANNEL_H

#include <netinet/in.h>
#include <sys/epoll.h>

#include "Antenna.h"

class Uplink_channel
{
    public:
        Uplink_channel(int port, double send_frequency);
        ~Uplink_channel();
        void run_timer_job();

    private:
        int port;
        int socket_fd;
        clock_t last_event_time;
        double send_frequency;
        struct sockaddr_in server_addr, client_addr;
        Antenna antenna;
        virtual void timer_job();

    protected:
        void send_message(void *message, size_t size);
};


#endif //SERVER_UPLINK_CHANNEL_H

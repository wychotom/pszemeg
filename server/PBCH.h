#ifndef PSZEMEG_PBCH_H
#define PSZEMEG_PBCH_H

#include <netinet/in.h>
#include <sys/epoll.h>

class PBCH
{
    public:
        PBCH(int port, size_t counter_reset);
        void send_mib();

        size_t getCounter();

    protected:
        size_t counter_reset;

    private:
        int port;
        int socket_fd;
        struct sockaddr_in server_addr, client_addr;

        size_t counter;
};


#endif //PSZEMEG_PBCH_H

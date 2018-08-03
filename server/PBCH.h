#ifndef PSZEMEG_PBCH_H
#define PSZEMEG_PBCH_H

#include <netinet/in.h>
#include <sys/epoll.h>

class PBCH
{
    public:
        PBCH(int port);
        void send_MIB();

    private:
        int port;
        int socket_fd;
        struct sockaddr_in server_addr, client_addr;
};


#endif //PSZEMEG_PBCH_H

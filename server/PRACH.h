#ifndef PSZEMEG_PRACH_H
#define PSZEMEG_PRACH_H

#include "Channel.h"

class PRACH
{
    public:
        PRACH(int port);
        void receive_message(bool *receive_again, struct RANDOM_ACCESS_PREAMBLE *new_message);

    private:
        int port;
        int socket_fd;
        struct sockaddr_in server_addr, client_addr;

};


#endif //PSZEMEG_PRACH_H

#ifndef SERVER_PRACH_H
#define SERVER_PRACH_H

#include "Downlink_channel.h"

class PRACH : public Downlink_channel
{
    public:
        PRACH(int port);
        void receive_message() override;
    //void receive_message(bool *receive_again, struct RANDOM_ACCESS_PREAMBLE *new_message);


//    private:
//        int port;
//        int socket_fd;
//        struct sockaddr_in server_addr, client_addr;
};


#endif //SERVER_PRACH_H

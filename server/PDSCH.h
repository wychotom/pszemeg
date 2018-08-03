#ifndef SERVER_PDSCH_H
#define SERVER_PDSCH_H

#include "Channel.h"

class PDSCH : public Channel
{
    public:
        PDSCH(int port);
        void send_random_access_response(int ra_rnti, int socket_fd);

    private:
        int recv_message(int event_fd) override;
};


#endif //SERVER_PDSCH_H

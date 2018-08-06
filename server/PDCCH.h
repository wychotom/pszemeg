#ifndef SERVER_PDCCH_H
#define SERVER_PDCCH_H

#include "Channel.h"
#include "UE.h"
#include <vector>

class PDCCH : public Channel
{
    public:
        PDCCH(int port, size_t counter_reset);
        void send_dci_to_all(bool cqi_request);

    private:
        bool cqi_request;

        void send_message(int socket_fd) override;
        int recv_message(int event_fd) override;
};


#endif //SERVER_PDCCH_H

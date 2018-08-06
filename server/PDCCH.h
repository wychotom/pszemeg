#ifndef SERVER_PDCCH_H
#define SERVER_PDCCH_H

#include "Uplink_channel.h"

class PDCCH : public Uplink_channel
{
    public:
        PDCCH(int port, size_t counter_reset);
        void send_dci(bool cqi_request);
};


#endif //SERVER_PDCCH_H

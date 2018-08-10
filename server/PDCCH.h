#ifndef SERVER_PDCCH_H
#define SERVER_PDCCH_H

#include "Uplink_channel.h"

class PDCCH : public Uplink_channel
{
    public:
        PDCCH(int port, double send_frequency);

    private:
        void timer_job() override;
};


#endif //SERVER_PDCCH_H

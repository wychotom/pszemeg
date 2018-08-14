#ifndef SERVER_PBCH_H
#define SERVER_PBCH_H

#include "Uplink_channel.h"

class PBCH : public Uplink_channel
{
    public:
        PBCH(int port, double send_frequency);

    private:
        void timer_job() override;
};


#endif //SERVER_PBCH_H

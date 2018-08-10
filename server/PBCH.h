#ifndef PSZEMEG_PBCH_H
#define PSZEMEG_PBCH_H

#include "Uplink_channel.h"

class PBCH : public Uplink_channel
{
    public:
        PBCH(int port, double send_frequency);

    private:
        void timer_job() override;
};


#endif //PSZEMEG_PBCH_H

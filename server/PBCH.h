#ifndef PSZEMEG_PBCH_H
#define PSZEMEG_PBCH_H

#include "Uplink_channel.h"

class PBCH : public Uplink_channel
{
    public:
        PBCH(int port);
        void send_mib();
};


#endif //PSZEMEG_PBCH_H

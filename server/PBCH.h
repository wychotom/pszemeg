#ifndef PSZEMEG_PBCH_H
#define PSZEMEG_PBCH_H

#include "Uplink_channel.h"

class PBCH : public Uplink_channel
{
    public:
        PBCH(int port, size_t counter_reset);
        void send_mib();
        size_t get_counter();

    private:
        size_t counter;
        size_t counter_reset;
};


#endif //PSZEMEG_PBCH_H

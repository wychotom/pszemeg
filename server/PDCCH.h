#ifndef SERVER_PDCCH_H
#define SERVER_PDCCH_H

#include "Uplink_channel.h"
#include "UE.h"

#include <vector>

class PDCCH : public Uplink_channel
{
    public:
        PDCCH(int port, double send_frequency, std::vector<UE*> &clients);

    private:
        void timer_job() override;
        std::vector<UE*> &clients;
};


#endif //SERVER_PDCCH_H

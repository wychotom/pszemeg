#ifndef SERVER_PUCCH_H
#define SERVER_PUCCH_H

#include "Downlink_channel.h"
#include "UE.h"

#include <vector>

class PUCCH : public Downlink_channel
{
    public:
        PUCCH(int port, std::vector<UE*> &clients);
        ssize_t receive_message(int event_fd) override;

    private:
        std::vector<UE*> &clients;
};


#endif //SERVER_PUCCH_H

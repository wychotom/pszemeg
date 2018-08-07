#ifndef SERVER_PRACH_H
#define SERVER_PRACH_H

#include "Downlink_channel.h"
#include "UE.h"

#include <vector>
#include <queue>

class PRACH : public Downlink_channel
{
    public:
        PRACH(int port, std::queue<UE*> &ue_queue, std::vector<UE*> &clients);
        void receive_message() override;

    private:
        std::queue<UE*> &ue_queue;
        std::vector<UE*> &clients;
};


#endif //SERVER_PRACH_H

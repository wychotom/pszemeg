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
        ssize_t receive_message(int event_fd) override;

    private:
        std::queue<UE*> &ue_queue;
        std::vector<UE*> &clients;
};


#endif //SERVER_PRACH_H

#ifndef SERVER_PDSCH_H
#define SERVER_PDSCH_H

#include "Uplink_channel.h"
#include "UE.h"
#include <queue>

class PDSCH : public Uplink_channel
{
    public:
        PDSCH(int port);
        void handle_queue(std::queue<UE*> &ue_queue);

    private:
        void send_random_access_response(UE &ue);
};


#endif //SERVER_PDSCH_H

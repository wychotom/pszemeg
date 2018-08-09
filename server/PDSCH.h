#ifndef SERVER_PDSCH_H
#define SERVER_PDSCH_H

#include "Uplink_channel.h"
#include "UE.h"
#include <vector>

class PDSCH : public Uplink_channel
{
    public:
        PDSCH(int port);
        void handle_queue(std::vector<UE*> &ue_vector);

    private:
        void send_random_access_response(UE &ue);
        UE* get_next_ue(const std::vector<UE*> &ue_vector, Action_to_perform action);
};


#endif //SERVER_PDSCH_H

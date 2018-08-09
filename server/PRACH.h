#ifndef SERVER_PRACH_H
#define SERVER_PRACH_H

#include "Downlink_channel.h"
#include "UE.h"

#include <vector>

class PRACH : public Downlink_channel
{
    public:
        PRACH(int port, std::vector<UE*> &ue_to_handle, std::vector<UE*> &clients);
        ssize_t receive_message(int event_fd) override;

    private:
        std::vector<UE*> &ue_to_handle;
        std::vector<UE*> &clients;
};


#endif //SERVER_PRACH_H

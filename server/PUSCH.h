#ifndef PSZEMEG_PUSCH_H
#define PSZEMEG_PUSCH_H

#include "Downlink_channel.h"
#include "UE.h"

#include <vector>

class PUSCH : public Downlink_channel
{
    public:
        PUSCH(int port, std::vector<UE*> &ue_to_handle, std::vector<UE*> &clients);
        ssize_t receive_message(int event_fd) override;

    private:
        std::vector<UE*> &ue_to_handle;
        std::vector<UE*> &clients;
};


#endif //PSZEMEG_PUSCH_H

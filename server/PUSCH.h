#ifndef PSZEMEG_PUSCH_H
#define PSZEMEG_PUSCH_H

#include "Downlink_channel.h"

class PUSCH : public Downlink_channel
{
    public:
        PUSCH(int port);
        ssize_t receive_message(int event_fd) override;

};


#endif //PSZEMEG_PUSCH_H

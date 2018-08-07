#ifndef PSZEMEG_PUSCH_H
#define PSZEMEG_PUSCH_H

#include "Downlink_channel.h"

class PUSCH : public Downlink_channel
{
    public:
        PUSCH(int port);
        void receive_message() override;

};


#endif //PSZEMEG_PUSCH_H

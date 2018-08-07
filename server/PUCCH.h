#ifndef PSZEMEG_PUCCH_H
#define PSZEMEG_PUCCH_H

#include "Downlink_channel.h"

class PUCCH : public Downlink_channel
{
    public:
        PUCCH(int port);
        void receive_message() override;

};


#endif //PSZEMEG_PUCCH_H

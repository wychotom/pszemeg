#ifndef SERVER_PRACH_H
#define SERVER_PRACH_H

#include "Downlink_channel.h"

class PRACH : public Downlink_channel
{
    public:
        PRACH(int port);
        ssize_t receive_message(int event_fd) override;
};


#endif //SERVER_PRACH_H

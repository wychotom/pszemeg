#ifndef PSZEMEG_SRB_H
#define PSZEMEG_SRB_H

#include "Dedicated_bearer.h"

class SRB
{
    public:
        SRB(int port);
        ssize_t receive_message(int event_fd) override;

};


#endif //PSZEMEG_SRB_H

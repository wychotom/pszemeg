#ifndef SERVER_SRB_H
#define SERVER_SRB_H

#include "Dedicated_bearer.h"

class SRB : public Dedicated_bearer
{
    public:
        SRB(int port);

    private:
        ssize_t recv_message(int event_fd) override;
};


#endif //SERVER_SRB_H

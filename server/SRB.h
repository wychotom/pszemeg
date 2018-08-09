#ifndef SERVER_SRB_H
#define SERVER_SRB_H

#include "Dedicated_bearer.h"
#include "UE.h"

#include <vector>

class SRB : public Dedicated_bearer
{
    public:
        SRB(int port, std::vector<UE*> &clients);

    private:
        ssize_t recv_message(int event_fd) override;
        std::vector<UE*> &clients;
};


#endif //SERVER_SRB_H

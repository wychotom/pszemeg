#ifndef SERVER_DRB_H
#define SERVER_DRB_H

#include "Dedicated_bearer.h"
#include "UE.h"

#include <vector>

class DRB : public Dedicated_bearer
{
    public:
        DRB(int port, std::vector<UE*> &clients);

    private:
        ssize_t recv_message(int event_fd) override;
        std::vector<UE*> &clients;
};


#endif //SERVER_DRB_H

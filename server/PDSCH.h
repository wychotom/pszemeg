#ifndef SERVER_PDSCH_H
#define SERVER_PDSCH_H

#include "Channel.h"
#include <queue>

class PDSCH : public Channel
{
    public:
        PDSCH(int port, std::queue<int> &ue_queue);
        void handle_queue();

    private:
        void send_random_access_response(int ra_rnti, int socket_fd);
        int recv_message(int event_fd) override;
        std::queue<int> &ue_queue;
};


#endif //SERVER_PDSCH_H

#include "PRACH.h"
#include "channels_struct.h"
#include "Downlink_channel.h"
#include "UE.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include <vector>
#include <queue>
#include <algorithm>

PRACH::PRACH(int port, std::queue<UE*> &ue_queue, std::vector<UE*> &clients) : Downlink_channel(port), ue_queue(ue_queue), clients(clients)
{
}

ssize_t PRACH::receive_message(int event_fd)
{
    struct RANDOM_ACCESS_PREAMBLE rap = {};

    ssize_t received_bytes = recv(event_fd, &rap, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0);

    if(received_bytes > 0)
    {
        std::cout << "received RAP " << rap.RA_RNTI << std::endl;

        auto it = std::find_if(clients.begin(), clients.end(), [this, &rap]( UE* client) {
            return client->RA_RNTI == rap.RA_RNTI;
        });

        if (it == clients.end())
        {
            UE *new_client = new UE(rap.RA_RNTI);
            clients.push_back(new_client);
            ue_queue.push(new_client);
        }
    }

    return received_bytes;
}

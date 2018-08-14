#include "PRACH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>

PRACH::PRACH(int port, std::vector<UE*> &ue_to_handle, std::vector<UE*> &clients) : Downlink_channel(port), ue_to_handle(ue_to_handle), clients(clients)
{
}

ssize_t PRACH::receive_message(int event_fd)
{
    struct RANDOM_ACCESS_PREAMBLE rap = {};

    ssize_t received_bytes = recv(event_fd, &rap, sizeof(struct RANDOM_ACCESS_PREAMBLE), MSG_DONTWAIT);

    if(received_bytes > 0)
    {
        Log::info("PRACH", "received \033[1;32mRAP\033[0m from " + std::to_string(rap.RA_RNTI));

        auto first_occurence_iterator = std::find_if(clients.begin(), clients.end(), [this, &rap](UE* client) {
            return client->RA_RNTI == rap.RA_RNTI;
        });

        if (first_occurence_iterator == clients.end())
        {
            UE *new_client = new UE(rap.RA_RNTI);
            new_client->set_action(Action_to_perform::random_access_response);
            clients.push_back(new_client);
            ue_to_handle.push_back(new_client);
        }
    }

    return received_bytes;
}

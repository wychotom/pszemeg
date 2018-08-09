#include "PRACH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"

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

    ssize_t received_bytes = recv(event_fd, &rap, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0);

    if(received_bytes > 0)
    {
        std::cout << "\033[1;33m[PRACH]\033[0m received \033[1;32mRAP\033[0m from " << rap.RA_RNTI << std::endl;

        auto first_occurence_iterator = std::find_if(clients.begin(), clients.end(), [this, &rap](UE* client) {
            return client->RA_RNTI == rap.RA_RNTI;
        });

        if (first_occurence_iterator == clients.end())
        {
            UE *new_client = new UE(rap.RA_RNTI);
            new_client->set_flag(Action_to_perform::random_access_response);
            clients.push_back(new_client);
            ue_to_handle.push_back(new_client);
        }
    }

    return received_bytes;
}

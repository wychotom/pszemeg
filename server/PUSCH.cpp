#include "PUSCH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <algorithm>
#include <vector>

PUSCH::PUSCH(int port, std::vector<UE*> &ue_to_handle, std::vector<UE*> &clients) : Downlink_channel(port), ue_to_handle(ue_to_handle), clients(clients)
{
}

ssize_t PUSCH::receive_message(int event_fd)
{
    RRC_CONN_REQUEST rcr = {};

    ssize_t received_bytes = recv(event_fd, &rcr, sizeof(RRC_CONN_REQUEST), 0);

    if(received_bytes > 0)
    {
        std::cout << "\033[1;33m[PUSCH]\033[0m received \033[0;35mRRC\033[0m Request from " << rcr.C_RNTI << std::endl;

        auto first_client_occurence_iterator = std::find_if(clients.begin(), clients.end(), [this, &rcr](UE* client) {
            return client->C_RNTI == rcr.C_RNTI;
        });

        if (first_client_occurence_iterator != clients.end())
        {
            (*first_client_occurence_iterator)->set_flag(Action_to_perform::rrc_connection_response);
            ue_to_handle.push_back(*first_client_occurence_iterator);
        }

    }

    return received_bytes;
}
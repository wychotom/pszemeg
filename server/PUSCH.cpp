#include "PUSCH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"
#include "Log.h"

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

    ssize_t received_bytes = recv(event_fd, &rcr, sizeof(RRC_CONN_REQUEST), MSG_DONTWAIT);

    if(received_bytes > 0)
    {
        Log::info("PUSCH", "received " + Log::colors[Colors::Green] + "RRC" + Log::colors[Colors::Default] + " Request from " + std::to_string(rcr.C_RNTI));

        auto first_client_occurence_iterator = std::find_if(clients.begin(), clients.end(), [this, &rcr](UE* client) {
            return client->C_RNTI == rcr.C_RNTI;
        });

        if (first_client_occurence_iterator != clients.end())
        {
            (*first_client_occurence_iterator)->set_action(Action_to_perform::rrc_connection_response);
            ue_to_handle.push_back(*first_client_occurence_iterator);
        }

    }

    return received_bytes;
}
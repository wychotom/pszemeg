#include "SRB.h"
#include "../common_header.h"
#include "Dedicated_bearer.h"
#include "UE.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

SRB::SRB(int port, std::vector<UE*> &clients) : Dedicated_bearer(port, clients), clients(clients)
{
}

ssize_t SRB::recv_message(int event_fd)
{
    struct RRC_CONN_SETUP_COMPLETE csc = {};

    ssize_t received_bytes = recv(event_fd, &csc, sizeof(struct RRC_CONN_SETUP_COMPLETE), 0);

    if(received_bytes > 0)
    {
        auto first_client_occurence_iterator = std::find_if(clients.begin(), clients.end(), [this, &csc](UE* client) {
            return client->C_RNTI == csc.C_RNTI;
        });

        if (first_client_occurence_iterator != clients.end())
        {
            (*first_client_occurence_iterator)->set_socket_fd(event_fd);
        }

        Log::info("SRB", "received " + Log::colors[Colors::Red] + "CSC" + Log::colors[Colors::Default] + " from " + std::to_string(csc.C_RNTI));
    }

    return received_bytes;
}
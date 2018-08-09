#include "SRB.h"
#include "../common_header.h"
#include "Dedicated_bearer.h"
#include "UE.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
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

        std::cout << "  \033[1;33m[SRB]\033[0m received \033[1;31mCSC\033[0m from " << csc.C_RNTI << std::endl;
    }

    return received_bytes;
}
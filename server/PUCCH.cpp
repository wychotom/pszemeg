#include "PUCCH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <algorithm>

PUCCH::PUCCH(int port, std::vector<UE*> &clients) : Downlink_channel(port), clients(clients)
{
}

ssize_t PUCCH::receive_message(int event_fd)
{
    struct UPLINK_CONTROL_INFORMATION uci = {};

    ssize_t received_bytes = recv(event_fd, &uci, sizeof(struct UPLINK_CONTROL_INFORMATION), 0);

    if(received_bytes > 0)
    {
        std::cout << "\033[1;33m[PUCCH]\033[0m received UCI form " << uci.ue_info.RNTI << std::endl;

        auto it = std::find_if(clients.begin(), clients.end(), [this, &uci]( UE* client) {
            return client->C_RNTI == uci.C_RNTI;
        });

        if (it != clients.end())
        {
            int index = (int)std::distance(clients.begin(), it);
            clients[index]->setSrb_identity(uci.ue_info.srb_identity);
            clients[index]->setTiming_advance(uci.ue_info.timing_advance);
            clients[index]->setUE_state(uci.ue_info.UE_state);
            clients[index]->setUl_sch_config(uci.ue_info.ul_sch_config);
            clients[index]->setUplink_power_control(uci.ue_info.uplink_power_control);
            clients[index]->setUplink_resource_grant(uci.ue_info.uplink_resource_grant);
        }

    }

    return received_bytes;
}
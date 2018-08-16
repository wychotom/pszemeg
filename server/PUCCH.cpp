#include "PUCCH.h"
#include "../common_header.h"
#include "Downlink_channel.h"
#include "UE.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

PUCCH::PUCCH(int port, std::vector<UE*> &clients) : Downlink_channel(port), clients(clients)
{
}

ssize_t PUCCH::receive_message(int event_fd)
{
    struct UPLINK_CONTROL_INFORMATION uci = {};

    ssize_t received_bytes = recv(event_fd, &uci, sizeof(struct UPLINK_CONTROL_INFORMATION), MSG_DONTWAIT);

    if(received_bytes > 0)
    {
        Log::info("PUCCH", "received " + Log::colors[Colors::Cyan] + "UCI" + Log::colors[Colors::Default] + " from " + std::to_string(uci.ue_info.RNTI));

        auto it = std::find_if(clients.begin(), clients.end(), [this, &uci]( UE* client) {
            return client->C_RNTI == uci.C_RNTI;
        });

        if (it != clients.end())
        {
            int index = (int)std::distance(clients.begin(), it);
            clients[index]->set_srb_identity(uci.ue_info.srb_identity);
            clients[index]->set_drb_identity(uci.ue_info.drb_identity);
            clients[index]->set_timing_advance(uci.ue_info.timing_advance);
            clients[index]->set_UE_state(uci.ue_info.UE_state);
            clients[index]->set_pusch_config(uci.ue_info.ul_sch_config);
            clients[index]->set_uplink_power_control(uci.ue_info.uplink_power_control);
            clients[index]->set_uplink_resource_grant(uci.ue_info.uplink_resource_grant);
            clients[index]->set_battery_life(uci.ue_info.battery_life);
            clients[index]->set_sleep_start(clock());
        }

    }

    return received_bytes;
}
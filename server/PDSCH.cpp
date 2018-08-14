#include "PDSCH.h"
#include "../common_header.h"
#include "Uplink_channel.h"
#include "UE.h"
#include "ports.h"
#include "Log.h"

#include <iostream>
#include <vector>
#include <algorithm>

PDSCH::PDSCH(int port) : Uplink_channel(port, 0)
{
}

void PDSCH::send_random_access_response(UE &ue)
{
    struct RANDOM_ACCESS_RESPONSE rar_message = {};

    rar_message.checksum = 0;
    rar_message.RA_RNTI = ue.RA_RNTI;
    rar_message.checksum += (long)rar_message.RA_RNTI;
    rar_message.temporary_c_rnti = ue.C_RNTI;
    rar_message.checksum += (long)rar_message.temporary_c_rnti;
    rar_message.timing_advance = 34;
    rar_message.checksum += (long)rar_message.timing_advance;
    rar_message.uplink_resource_grant = 523;
    rar_message.checksum += (long)rar_message.uplink_resource_grant;

    send_message((void*) &rar_message, sizeof(struct RANDOM_ACCESS_RESPONSE));

    Log::info("PDSCH", Log::colors[Colors::Magenta] + "RAR" + Log::colors[Colors::Default] + " sent to " + std::to_string(ue.RA_RNTI ));
}

void PDSCH::send_rrc_connection_response(UE &ue)
{
    struct RRC_CONN_SETUP rcr_message = {};

    rcr_message.C_RNTI = ue.C_RNTI;
    rcr_message.dl_am_rlc = 0;
    rcr_message.phr_config = 1;
    rcr_message.srb_identity = Ports::srb_port;
    rcr_message.ul_am_rlc = 1;
    rcr_message.ul_sch_config = 2;

    DRX_CONFIG drx_config = {};
    drx_config.on_duration_timer = 2;
    drx_config.short_drx_timer = 5;
    drx_config.long_drx_timer = 10;
    drx_config.drx_cycle_type = 0;

    rcr_message.uplink_power_control = drx_config;

    send_message((void*) &rcr_message, sizeof(struct RRC_CONN_SETUP));

    Log::info("PDSCH", Log::colors[Colors::Green] + "RCR" + Log::colors[Colors::Default] + " sent to " + std::to_string(ue.C_RNTI));
}

void PDSCH::handle_queue(std::vector<UE*> &ue_vector)
{
    UE *ue = nullptr;
    Action_to_perform rar_action = Action_to_perform::random_access_response;
    Action_to_perform rcr_action = Action_to_perform::rrc_connection_response;

    while((ue = get_next_ue(ue_vector, rar_action)) != nullptr)
    {
        ue->C_RNTI = 100 * ue->RA_RNTI;
        send_random_access_response(*ue);
    }

    while((ue = get_next_ue(ue_vector, rcr_action)) != nullptr)
    {
        send_rrc_connection_response(*ue);
    }
}

UE* PDSCH::get_next_ue(std::vector<UE*> &ue_vector, Action_to_perform action)
{
    auto it = std::find_if(ue_vector.begin(), ue_vector.end(), [&action] (UE* client)
    {
        return client->get_action() == action && client->is_transmission_possible();
    });

    if(it == ue_vector.end())
    {
        return nullptr;
    }

    UE *ue = *it;

    ue_vector.erase(it);

    return ue;
}

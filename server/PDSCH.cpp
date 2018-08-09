#include "PDSCH.h"
#include "../common_header.h"
#include "Uplink_channel.h"
#include "UE.h"

#include <iostream>
#include <vector>
#include <algorithm>

PDSCH::PDSCH(int port) : Uplink_channel(port, 0)
{
}

void PDSCH::send_random_access_response(UE &ue)
{
    struct RANDOM_ACCESS_RESPONSE rar_message;

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

    std::cout << "\033[1;33m[PDSCH]\033[0m RAR sent to " << ue.RA_RNTI << std::endl;
}

void PDSCH::handle_queue(std::vector<UE*> &ue_vector)
{
    UE *ue;
    Action_to_perform rar_action = Action_to_perform::random_access_response;

    while((ue = get_next_ue(ue_vector, rar_action)) != nullptr)
    {
        ue->C_RNTI = 100 * ue->RA_RNTI;
        send_random_access_response(*ue);
    }
}

UE* PDSCH::get_next_ue(const std::vector<UE*> &ue_vector, Action_to_perform action)
{
    auto it = std::find_if(ue_vector.cbegin(), ue_vector.cend(), [&action] (UE* client)
    {
        return client->get_flag() == action;
    });

    if(it == ue_vector.end())
    {
        return nullptr;
    }

    return *it;
}

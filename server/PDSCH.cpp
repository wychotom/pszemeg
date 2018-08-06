#include "PDSCH.h"
#include "channels_struct.h"
#include "Uplink_channel.h"
#include "UE.h"

#include <iostream>
#include <queue>

PDSCH::PDSCH(int port) : Uplink_channel(port, 0)
{
}

void PDSCH::send_random_access_response(UE &ue)
{
    struct RANDOM_ACCESS_RESPONSE_MESSAGE rar_message;
    rar_message.ra_rnti = ue.RA_RNTI;
    rar_message.temporary_c_rnti = ue.C_RNTI;
    rar_message.timing_advance = 34;
    rar_message.uplink_resource_grant = 523;

    send_message((void*) &rar_message, sizeof(struct RANDOM_ACCESS_RESPONSE_MESSAGE));

    std::cout << "RAR sent to" << std::endl;
}

void PDSCH::handle_queue(std::queue<UE*> &ue_queue)
{
    while(!ue_queue.empty())
    {
        UE *ue = ue_queue.front();
        ue_queue.pop();

        ue->C_RNTI = rand() * 100 * ue->RA_RNTI;
        send_random_access_response(*ue);
    }
}

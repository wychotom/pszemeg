#include "PBCH.h"
#include "channels_struct.h"
#include "Uplink_channel.h"

PBCH::PBCH(int port, size_t counter_reset) : Uplink_channel(port), counter(1)
{
    this->counter_reset = counter_reset;
}

void PBCH::send_mib()
{
    struct MIB_MESSAGE new_message = {};

    new_message.broadcast_port = 20700;
    new_message.dl_sch_port = 20701;
    new_message.ul_sch_port = 20702;
    new_message.pdcch_port = 20703;
    new_message.pucch_port = 20704;
    new_message.prach_port = 20705;

    send_message((void*) &new_message, sizeof(struct MIB_MESSAGE));
}

size_t PBCH::get_counter()
{
    if(++this->counter >= this->counter_reset)
    {
        this->counter = 0;
    }

    return this->counter;
}

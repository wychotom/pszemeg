#include "PBCH.h"
#include "../common_header.h"
#include "Uplink_channel.h"
#include "Log.h"

#include <iostream>

PBCH::PBCH(int port, double send_frequency) : Uplink_channel(port, send_frequency)
{
}

void PBCH::timer_job()
{
    struct MIB_MESSAGE mib_message = {};

    mib_message.broadcast_port = 20700;
    mib_message.dl_sch_port = 20701;
    mib_message.ul_sch_port = 20702;
    mib_message.pdcch_port = 20703;
    mib_message.pucch_port = 20704;
    mib_message.prach_port = 20705;

    send_message((void*) &mib_message, sizeof(struct MIB_MESSAGE));

    Log::info("PBCH", "MIB sent");
}

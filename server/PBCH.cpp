#include "PBCH.h"
#include "channels_struct.h"
#include "Uplink_channel.h"

#include <iostream>

PBCH::PBCH(int port, size_t counter_reset) : Uplink_channel(port, counter_reset)
{
}

void PBCH::send_mib()
{
    struct MIB_MESSAGE mib_message = {};

    mib_message.checksum = 0;
    mib_message.broadcast_port = 20700;
    mib_message.checksum += (long)mib_message.broadcast_port;
    mib_message.dl_sch_port = 20701;
    mib_message.checksum += (long)mib_message.dl_sch_port;
    mib_message.ul_sch_port = 20702;
    mib_message.checksum += (long)mib_message.ul_sch_port;
    mib_message.pdcch_port = 20703;
    mib_message.checksum += (long)mib_message.pdcch_port;
    mib_message.pucch_port = 20704;
    mib_message.checksum += (long)mib_message.pucch_port;
    mib_message.prach_port = 20705;
    mib_message.checksum += (long)mib_message.prach_port;

    send_message((void*) &mib_message, sizeof(struct MIB_MESSAGE));

    std::cout << "[PBCH]  MIB sent" << std::endl;
}

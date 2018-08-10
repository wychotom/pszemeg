#include "PDCCH.h"
#include "../common_header.h"
#include "Uplink_channel.h"

#include <iostream>

PDCCH::PDCCH(int port, double send_frequency) : Uplink_channel(port, send_frequency)
{
}

void PDCCH::timer_job()
{
    struct DCI_MESSAGE dci_message;

    dci_message.checksum = 0;
    dci_message.format0_a_flag = 1;
    dci_message.checksum += (long)dci_message.format0_a_flag;
    dci_message.freqency_hooping = 0;
    dci_message.checksum += (long)dci_message.freqency_hooping;
    dci_message.riv = 15;
    dci_message.checksum += (long)dci_message.riv;
    dci_message.mcs = 20;
    dci_message.checksum += (long)dci_message.mcs;
    dci_message.ndi = 1;
    dci_message.checksum += (long)dci_message.ndi;
    dci_message.tpc = 88;
    dci_message.checksum += (long)dci_message.tpc;
    dci_message.cyclic_shift = 34;
    dci_message.checksum += (long)dci_message.cyclic_shift;
    dci_message.cqi_request = 1;
    dci_message.checksum += (long)dci_message.cqi_request;

    send_message((void*) &dci_message, sizeof(struct DCI_MESSAGE));

    std::cout << "\033[1;33m[PDCCH]\033[0m DCI sent" << std::endl;
}




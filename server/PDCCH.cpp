#include "PDCCH.h"
#include "../common_header.h"
#include "Uplink_channel.h"
#include "UE.h"

#include <iostream>
#include <vector>

PDCCH::PDCCH(int port, double send_frequency, std::vector<UE*> &clients) : Uplink_channel(port, send_frequency), clients(clients)
{
}

void PDCCH::timer_job()
{
    for(UE* client : clients)
    {
        if(!client->is_transmission_possible() || client->get_socket_fd() == 0 || client->get_battery_life() == -1)
        {
            continue;
        }

        struct DCI_MESSAGE dci_message = {};

        dci_message.checksum = 0;
        dci_message.C_RNTI = client->C_RNTI;
        dci_message.checksum += (long)dci_message.C_RNTI;
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

        dci_message.drx_config.on_duration_timer = 2;
        dci_message.drx_config.short_drx_timer = 5;
        dci_message.drx_config.long_drx_timer = 10;

        if(client->get_battery_life() > 30)
        {
            dci_message.drx_config.drx_cycle_type = 0; // short cycle
        }
        else
        {
            dci_message.drx_config.drx_cycle_type = 1; // long cycle
        }

        dci_message.checksum += (long)dci_message.drx_config.on_duration_timer;
        dci_message.checksum += (long)dci_message.drx_config.short_drx_timer;
        dci_message.checksum += (long)dci_message.drx_config.long_drx_timer;
        dci_message.checksum += (long)dci_message.drx_config.drx_cycle_type;

        send_message((void*) &dci_message, sizeof(struct DCI_MESSAGE));
        std::cout << "\033[1;33m[PDCCH]\033[0m DCI sent" << std::endl;
    }
}




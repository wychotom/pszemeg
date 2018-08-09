#include "eNodeB.h"
#include "channels_struct.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PDSCH.h"
#include "PRACH.h"

#include <vector>
#include <iostream>

eNodeB::eNodeB() :
pbch(Ports::pbch_port, 0x3FFFFF),
pdcch(Ports::pdcch_port, 0xFFFFFF),
pdsch(Ports::pdsch_port),
prach(Ports::prach_port, ue_queue, clients),
pusch(Ports::pusch_port),
pucch(Ports::pucch_port, clients)
{
}

void eNodeB::start()
{
    while(true)
    {
        if(this->pbch.get_counter() == 0)
        {
            this->pbch.send_mib();
        }

        if(this->pdcch.get_counter() == 0)
        {
            this->pdcch.send_dci(true);
        }

        this->prach.handle_connections();

        this->pdsch.handle_queue(this->ue_queue);

        this->pusch.handle_connections();
        this->pucch.handle_connections();
    }
}
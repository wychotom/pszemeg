#include "eNodeB.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "SRB.h"
#include "ports.h"
#include "Antenna.h"

#include <vector>
#include <iostream>

bool eNodeB::is_running = true;

eNodeB::eNodeB() :
pbch(Ports::pbch_port, 0.5),
pdcch(Ports::pdcch_port, 1, clients),
pdsch(Ports::pdsch_port),
prach(Ports::prach_port, ue_to_handle, clients),
pusch(Ports::pusch_port, ue_to_handle, clients),
pucch(Ports::pucch_port, clients),
srb(Ports::srb_port, clients)
{
}

void eNodeB::start()
{
    while(eNodeB::is_running)
    {
        antenna.render_antenna();

        this->pbch.run_timer_job();

        this->pdcch.run_timer_job();

        this->prach.handle_connections();

        this->pdsch.handle_queue(this->ue_to_handle);

        this->pusch.handle_connections();

        this->pucch.handle_connections();

        this->srb.handle_connections();
    }
}

eNodeB::~eNodeB()
{
    for(auto ue : this->clients)
    {
        delete ue;
    }

    for(auto ue : this->ue_to_handle)
    {
        delete ue;
    }
}

#include "eNodeB.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "SRB.h"
#include "DRB.h"
#include "ports.h"
#include "Antenna.h"

#include <vector>
#include <iostream>
#include <unistd.h>
#include <algorithm>

bool eNodeB::is_running = true;

eNodeB::eNodeB() :
pbch(Ports::pbch_port, 0.5),
pdcch(Ports::pdcch_port, 1, clients),
pdsch(Ports::pdsch_port),
prach(Ports::prach_port, ue_to_handle, clients),
pusch(Ports::pusch_port, ue_to_handle, clients),
pucch(Ports::pucch_port, clients),
srb(Ports::srb_port, clients),
drb(Ports::drb_port, clients)
{
}

void eNodeB::start()
{
    while(eNodeB::is_running)
    {
        #ifndef DEBUG
            antenna.render_antenna(clients.size());
        #endif

        this->pbch.run_timer_job();

        this->pdcch.run_timer_job();

        this->prach.handle_connections();

        this->pdsch.handle_queue(this->ue_to_handle);

        this->pusch.handle_connections();

        this->pucch.handle_connections();

        this->srb.handle_connections();

        this->drb.handle_connections();

        for(auto client : clients)
        {
            double elapsed_secs = double(clock()  - client->get_last_response_time()) / CLOCKS_PER_SEC;

            if(elapsed_secs > 3 && client->get_socket_fd() == 0) //timeout
            {
                clients.erase(std::find_if(clients.begin(), clients.end(), [&client](UE* ue)
                    {
                        return ue == client;
                    }));
            }
        }
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

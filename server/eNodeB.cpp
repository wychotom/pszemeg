#include "eNodeB.h"
#include "channels_struct.h"

#include <vector>
#include <iostream>

eNodeB::eNodeB() :
pbch(PORTS::pbch_port, 0x7FFFFFF),
pdcch(PORTS::pdcch_port, 0xFFFFFFF)
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
    }
}
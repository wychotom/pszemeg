#include "eNodeB.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() : pbch(20700, 0xFFFFFFF)
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

//        this->pdcch.handle_connections();
//
//        if(this->pdcch.getCounter() == 0)
//        {
//            this->pdcch.send_dci_to_all(true);
//        }
    }
}
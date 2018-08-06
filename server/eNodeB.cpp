#include "eNodeB.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() : pbch(20700)
{
}

void eNodeB::start()
{
    int MIB_iterator = 0;

    while(true)
    {
        MIB_iterator++;

        if(MIB_iterator == 10000000)
        {
            MIB_iterator = 0;
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
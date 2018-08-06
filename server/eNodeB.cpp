#include "eNodeB.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() : pdcch(20703, 0xFFFFFF), pbch(20700)
{
//    this->channels.push_back(new Channel(27000));
//    this->channels.push_back(new Channel(27001));
//    this->channels.push_back(new Channel(27002));
//    this->channels.push_back(new Channel(27003));

}

eNodeB::~eNodeB()
{
//    for(auto channel : this->channels)
//    {
//        delete channel;
//    }
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
            this->pbch.send_MIB();
        }

        this->pdcch.handle_connections();

        if(this->pdcch.getCounter() == 0)
        {
            this->pdcch.send_dci_to_all(true);
        }
    }
}
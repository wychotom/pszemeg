#include "eNodeB.h"
#include "Channel.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() : pdcch(20703, 0xFFFFFF), pbch(20700, 0xFFFFFF)
{
//    this->channels.push_back(new Channel(27000));
//    this->channels.push_back(new Channel(27001));
//    this->channels.push_back(new Channel(27002));
//    this->channels.push_back(new Channel(27003));

    Channel::clients = this->clients;

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
    while(true)
    {
        if(this->pbch.getCounter() == 0)
        {
            this->pbch.send_mib();
        }

        this->pdcch.handle_connections();

        if(this->pdcch.getCounter() == 0)
        {
            this->pdcch.send_dci_to_all(true);
        }
    }
}
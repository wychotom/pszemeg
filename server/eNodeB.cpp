#include "eNodeB.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() : pbch(27019)
{
    this->channels.push_back(new Channel(27020));
    this->channels.push_back(new Channel(27021));
    this->channels.push_back(new Channel(27022));
    this->channels.push_back(new Channel(27023));
}

eNodeB::~eNodeB()
{
    for(auto channel : this->channels)
    {
        delete channel;
    }
}

void eNodeB::start()
{
    int MIB_iterator = 0;

    while(true)
    {
        MIB_iterator++;

        if(MIB_iterator%1000000)
        {
            MIB_iterator = 0;
            this->pbch.send_MIB();
        }

        for(auto channel : this->channels)
        {
            channel->handle_connections();
        }
    }
}
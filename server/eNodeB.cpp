#include "eNodeB.h"
#include <vector>

eNodeB::eNodeB() : pdcch(27000, 0xFFFFFF)
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
    while(true)
    {
        this->pdcch.handle_connections();

        if(this->pdcch.getCounter() == 0)
        {
            this->pdcch.send_dci_to_all(true);
        }
    }
}
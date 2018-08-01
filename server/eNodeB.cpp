#include "eNodeB.h"
#include <vector>

eNodeB::eNodeB()
{
    this->channels.push_back(new Channel(27000));
    this->channels.push_back(new Channel(27001));
    this->channels.push_back(new Channel(27002));
    this->channels.push_back(new Channel(27003));
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
    while(true)
    {
        for(auto channel : this->channels)
        {
            channel->handle_connections();
        }
    }
}
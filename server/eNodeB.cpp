#include "eNodeB.h"

eNodeB::eNodeB()
{
    this->channels.emplace_back(27000);
    this->channels.emplace_back(27001);
    this->channels.emplace_back(27002);
    this->channels.emplace_back(27003);
}

void eNodeB::start()
{
    while(true)
    {
        for(auto &channel : this->channels)
        {
            channel.handle_connections();
        }
    }
}
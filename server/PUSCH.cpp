#include "PUSCH.h"
#include "channels_struct.h"
#include "Downlink_channel.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

PUSCH::PUSCH(int port) : Downlink_channel(port)
{
}

void PUSCH::receive_message()
{
    struct UPLINK_CONTROL_INFORMATION uci = {};

    ssize_t received_bytes = recvfrom(this->socket_fd, &uci, sizeof(struct UPLINK_CONTROL_INFORMATION), 0, nullptr, nullptr);

    if(received_bytes > 0)
    {
        std::cout << "UE RNTI " << uci.ue.RNTI << std::endl;
    }
}
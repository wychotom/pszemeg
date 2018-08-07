#include "PUSCH.h"
#include "channels_struct.h"
#include "Downlink_channel.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

PUSCH::PUSCH(int port) : Downlink_channel(port)
{
}

ssize_t PUSCH::receive_message(int event_fd)
{
    struct UPLINK_CONTROL_INFORMATION uci = {};

    ssize_t received_bytes = recv(event_fd, &uci, sizeof(struct UPLINK_CONTROL_INFORMATION), 0);

    if(received_bytes > 0)
    {
        std::cout << "received UCI " << uci.ue_info.RNTI << std::endl;
    }

    return received_bytes;
}
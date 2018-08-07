#include "PRACH.h"
#include "channels_struct.h"
#include "Downlink_channel.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

PRACH::PRACH(int port) : Downlink_channel(port)
{
}

ssize_t PRACH::receive_message(int event_fd)
{
    struct RANDOM_ACCESS_PREAMBLE rap = {};

    ssize_t received_bytes = recv(event_fd, &rap, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0);

    if(received_bytes > 0)
    {
        std::cout << "RAP " << rap.RA_RNTI << std::endl;
    }

    return received_bytes;
}

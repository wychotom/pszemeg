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
    std::string message(1024, 0);

    ssize_t received_bytes = recv(event_fd, &message, message.size(), 0);

    if(received_bytes > 0)
    {
        std::cout << "[PUSCH] received data: " << message << std::endl;
    }

    return received_bytes;
}
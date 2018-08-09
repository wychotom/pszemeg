#include "SRB.h"
#include "channels_struct.h"
#include "../common_header.h"
#include "Direct_bearer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

SRB::SRB(int port) : Direct_bearer(port)
{
}

ssize_t SRB::receive_message(int event_fd)
{
    struct CONNECTION_SETUP_COMPLETE csc = {};

    ssize_t received_bytes = recv(event_fd, &csc, sizeof(struct CONNECTION_SETUP_COMPLETE), 0);

    if(received_bytes > 0)
    {
        std::cout << "[SRB]   received CSC from " << csc.C_RNTI << std::endl;
    }

    return received_bytes;
}
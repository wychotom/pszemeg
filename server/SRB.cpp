#include "SRB.h"
#include "../common_header.h"
#include "Dedicated_bearer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

SRB::SRB(int port) : Dedicated_bearer(port)
{
}

ssize_t SRB::recv_message(int event_fd)
{
    struct RRC_CONN_SETUP_COMPLETE csc = {};

    ssize_t received_bytes = recv(event_fd, &csc, sizeof(struct RRC_CONN_SETUP_COMPLETE), 0);

    if(received_bytes > 0)
    {
        std::cout << "  \033[1;33m[SRB]\033[0m received CSC from " << csc.C_RNTI << std::endl;
    }

    return received_bytes;
}
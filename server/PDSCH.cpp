#include "PDSCH.h"
#include "channels_struct.h"
#include <iostream>

PDSCH::PDSCH(int port) : Channel(port, 0)
{
}

int PDSCH::recv_message(int event_fd)
{
    char c;
    int bytes_count = recv(event_fd, &c, 1, 0);

    if(bytes_count > 0)
    {
        throw std::string("PDSCH is not UL channel");
    }

    return bytes_count;
}

void PDSCH::send_random_access_response(int ra_rnti, int socket_fd)
{
    struct RANDOM_ACCESS_RESPONSE_MESSAGE rar_message;
    rar_message.ra_rnti = ra_rnti;
    rar_message.temporary_c_rnti = rand() * 100 * ra_rnti;
    rar_message.timing_advance = 34;
    rar_message.uplink_resource_grant = 523;

    if(send(socket_fd, &rar_message, sizeof(struct RANDOM_ACCESS_RESPONSE_MESSAGE), 0) < 0)
    {
        perror("ERROR: ");
        throw std::string("RAR send fail");
    }

    std::cout << "RAR sent to " << clients_fds[socket_fd] << std::endl;
}

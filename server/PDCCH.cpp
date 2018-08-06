#include <iostream>
#include "PDCCH.h"
#include "channels_struct.h"

PDCCH::PDCCH(int port, size_t counter_reset) : Channel(port, counter_reset)
{
    this->cqi_request = false;
}

void PDCCH::send_message(int socket_fd)
{
    struct DCI_MESSAGE dci_message;
    dci_message.format0_a_flag = 1;
    dci_message.freqency_hooping = 0;
    dci_message.riv = 15;
    dci_message.mcs = 20;
    dci_message.ndi = 1;
    dci_message.tpc = 88;
    dci_message.cyclic_shift = 34;
    dci_message.cqi_request = static_cast<char>(this->cqi_request ? 1 : 0);

    if(send(socket_fd, &dci_message, sizeof(struct DCI_MESSAGE), 0) < 0)
    {
        perror("ERROR: ");
        throw std::string("Send fail");
    }

    std::cout << "DCI sent to " << clients_fds[socket_fd] << std::endl;
}

int PDCCH::recv_message(int event_fd)
{
    char c;
    int bytes_count = recv(event_fd, &c, 1, 0);

    if(bytes_count > 0)
    {
        throw std::string("PDCCH is not UL channel");
    }

    return bytes_count;
}

void PDCCH::send_dci_to_all(bool cqi_request)
{
    this->cqi_request = cqi_request;

    for (auto const& client : clients_fds)
    {
        send_message(client.first);
    }
}




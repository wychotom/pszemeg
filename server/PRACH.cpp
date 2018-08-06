#include "PRACH.h"
#include "channels_struct.h"

#include <sys/types.h>
#include <sys/socket.h>

PRACH::PRACH(int port)
{
    this->port = port;

    this->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(this->socket_fd  == -1)
    {
        perror("ERROR: ");
        throw std::string("socket fail");
    }

    int broadcast = 1;

    if(setsockopt(this->socket_fd , SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1)
    {
        perror("ERROR: ");
        throw std::string("setsockopt fail");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(this->socket_fd , (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("ERROR: ");
        throw std::string("bind fail");
    }
}

void PRACH::receive_message(bool *receive_again, struct RANDOM_ACCESS_PREAMBLE *new_message)
{
    socklen_t client_addr_len = sizeof(this->client_addr);

    int received_bytes = recvfrom(this->socket_fd, new_message, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0,
                                  (struct sockaddr*) &this->client_addr, &client_addr_len);

    if(received_bytes > 0)
    {
        if (received_bytes > 1024)
        {
            *receive_again = true;
            return;
        }
        else
        {
            *receive_again = false;
            return;
        }
    }
    else if(received_bytes == 0)
    {
        perror("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
    else if(received_bytes == -1)
    {
        perror("ERROR: ");
        throw std::string("recvfrom fail");
    }
}

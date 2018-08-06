#include "UE.h"

UE::UE(int socket_fd)
{
    this->socket_fd = socket_fd;
}

void UE::set_RA_RNTI(int RA_RNTI)
{
    this->RA_RNTI = RA_RNTI;
}

void UE::set_C_RNTI(int C_RNTI)
{
    this->C_RNTI = C_RNTI;
}

int UE::get_socket_fd()
{
    return this->socket_fd;
}
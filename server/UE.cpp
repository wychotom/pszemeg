#include "UE.h"
#include "channels_struct.h"

UE::UE(Channel_flags flag)
{
    this->flag = flag;
}

void UE::set_RA_RNTI(int RA_RNTI)
{
    this->RA_RNTI = RA_RNTI;
}

void UE::set_C_RNTI(int C_RNTI)
{
    this->C_RNTI = C_RNTI;
}

int UE::get_flag()
{
    return this->flag;
}
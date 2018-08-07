#include "UE.h"
#include "channels_struct.h"

UE::UE(int RA_RNTI)
{
    this->RA_RNTI = RA_RNTI;
}

UE::UE(Channel_flags flag)
{
    this->flag = flag;
}

Channel_flags UE::get_flag()
{
    return this->flag;
}
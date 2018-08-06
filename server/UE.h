#ifndef SERVER_UE_H
#define SERVER_UE_H

#include "channels_struct.h"

class UE
{
    public:
        UE(Channel_flags flag);
        int RA_RNTI;
        int C_RNTI;
        Channel_flags get_flag();

    private:
        Channel_flags flag;
};


#endif //SERVER_UE_H

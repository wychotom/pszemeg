#ifndef SERVER_UE_H
#define SERVER_UE_H

#include "channels_struct.h"

class UE
{
    public:
        UE(Channel_flags flag);
        void set_RA_RNTI(int RA_RNTI);
        void set_C_RNTI(int C_RNTI);
        Channel_flags get_flag();

    private:
        int RA_RNTI;
        int C_RNTI;
        Channel_flags flag;
};


#endif //SERVER_UE_H

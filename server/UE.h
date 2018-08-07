#ifndef SERVER_UE_H
#define SERVER_UE_H

#include "channels_struct.h"

class UE
{
    public:
        UE(Channel_flags flag);
        UE(int RA_RNTI);
        int RA_RNTI;
        int C_RNTI;
        Channel_flags get_flag();

    private:
        Channel_flags flag;
        int UE_state;
        int timing_advance;
        int uplink_resource_grant;
        int uplink_power_control;
        int ul_sch_config;
        int srb_identity;
};


#endif //SERVER_UE_H

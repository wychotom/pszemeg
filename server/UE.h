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

        void setUE_state(int UE_state);
        void setTiming_advance(int timing_advance);
        void setUplink_resource_grant(int uplink_resource_grant);
        void setUplink_power_control(int uplink_power_control);
        void setUl_sch_config(int ul_sch_config);
        void setSrb_identity(int srb_identity);

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

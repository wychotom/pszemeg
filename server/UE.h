#ifndef SERVER_UE_H
#define SERVER_UE_H

#include "../common_header.h"

enum Action_to_perform
{
    no_action,
    random_access_response,
    rrc_connection_response
};

class UE
{
    public:
        UE(int RA_RNTI);
        int RA_RNTI;
        int C_RNTI;

        Action_to_perform get_action();
        void set_action(Action_to_perform flag);
        void set_UE_state(int UE_state);
        void set_timing_advance(int timing_advance);
        void set_uplink_resource_grant(int uplink_resource_grant);
        void set_uplink_power_control(struct DRX_CONFIG drx_config);
        void set_pusch_config(int ul_sch_config);
        void set_srb_identity(int srb_identity);
        void set_drb_identity(int drb_identity);
        int get_battery_life() const;
        void set_battery_life(int battery_life);
        int get_socket_fd() const;
        void set_socket_fd(int socket_fd);
        void set_sleep_start(clock_t sleep_start);
        bool is_transmission_possible();

    private:
        Action_to_perform flag;
        int socket_fd;
        int UE_state;
        int timing_advance;
        int uplink_resource_grant;
        struct DRX_CONFIG uplink_power_control;
        clock_t sleep_start;
        int ul_sch_config;
        int srb_identity;
        int drb_identity;
        int battery_life;
};

#endif //SERVER_UE_H

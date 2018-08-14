#include "UE.h"
#include <ctime>

UE::UE(int RA_RNTI)
{
    this->RA_RNTI = RA_RNTI;
    this->battery_life = -1;
    this->uplink_power_control = {};
    this->flag = Action_to_perform::no_action;
    this->socket_fd = 0;
    this->sleep_start = 0;
}

Action_to_perform UE::get_action()
{
    return this->flag;
}

void UE::set_UE_state(int UE_state)
{
    UE::UE_state = UE_state;
}

void UE::set_timing_advance(int timing_advance)
{
    UE::timing_advance = timing_advance;
}

void UE::set_uplink_resource_grant(int uplink_resource_grant)
{
    UE::uplink_resource_grant = uplink_resource_grant;
}

void UE::set_uplink_power_control(struct DRX_CONFIG drx_config)
{
    UE::uplink_power_control = drx_config;
}

void UE::set_pusch_config(int ul_sch_config)
{
    UE::ul_sch_config = ul_sch_config;
}

void UE::set_srb_identity(int srb_identity)
{
    UE::srb_identity = srb_identity;
}

void UE::set_action(Action_to_perform flag)
{
    UE::flag = flag;
}

void UE::set_socket_fd(int socket_fd)
{
    UE::socket_fd = socket_fd;
}

int UE::get_socket_fd() const
{
    return socket_fd;
}

int UE::get_battery_life() const
{
    return battery_life;
}

void UE::set_battery_life(int battery_life)
{
    UE::battery_life = battery_life;
}

void UE::set_sleep_start(clock_t sleep_start)
{
    UE::sleep_start = sleep_start;
}

bool UE::is_transmission_possible()
{
    clock_t now = clock();
    int on_duration_timer = this->uplink_power_control.on_duration_timer;
    int cycle_duration;

    if(this->uplink_power_control.drx_cycle_type == 0)
    {
        cycle_duration = this->uplink_power_control.short_drx_timer;
    }
    else
    {
        cycle_duration = this->uplink_power_control.long_drx_timer;
    }

    double elapsed_secs = double(now - this->sleep_start) / CLOCKS_PER_SEC;

    return elapsed_secs >= (cycle_duration - on_duration_timer);
}

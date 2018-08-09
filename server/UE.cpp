#include "UE.h"

UE::UE(int RA_RNTI)
{
    this->RA_RNTI = RA_RNTI;
}

UE::UE(Action_to_perform flag)
{
    this->flag = flag;
}

Action_to_perform UE::get_flag()
{
    return this->flag;
}

void UE::setUE_state(int UE_state)
{
    UE::UE_state = UE_state;
}

void UE::setTiming_advance(int timing_advance)
{
    UE::timing_advance = timing_advance;
}

void UE::setUplink_resource_grant(int uplink_resource_grant)
{
    UE::uplink_resource_grant = uplink_resource_grant;
}

void UE::setUplink_power_control(int uplink_power_control)
{
    UE::uplink_power_control = uplink_power_control;
}

void UE::setUl_sch_config(int ul_sch_config)
{
    UE::ul_sch_config = ul_sch_config;
}

void UE::setSrb_identity(int srb_identity)
{
    UE::srb_identity = srb_identity;
}

void UE::set_flag(Action_to_perform flag)
{
    UE::flag = flag;
}

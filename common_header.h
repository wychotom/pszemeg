#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

//#include <time.h>

typedef long int clock_t;
struct MIB_MESSAGE;
struct RANDOM_ACCESS_PREAMBLE;
struct RANDOM_ACCESS_RESPONSE;
struct DRX_CONFIG;
struct DCI_MESSAGE;
struct UE_INFO;
struct UPLINK_CONTROL_INFORMATION;
struct RRC_CONN_REQUEST;
struct RRC_CONN_SETUP;
struct RRC_CONN_SETUP_COMPLETE;

enum establishment_causes
{
    emergency,
    high_priority_access,
    mobile_terminating_access,
    mobile_originating_signaling,
    mobile_originating_data
};

struct MIB_MESSAGE
{
    int prach_port;
    int broadcast_port;
    int dl_sch_port;
    int ul_sch_port; 
    int pdcch_port;
    int pucch_port;
    long checksum;
};

struct RANDOM_ACCESS_PREAMBLE//prach
{
    int RA_RNTI;
    short int preamble; //1337
    long checksum;
};

struct RANDOM_ACCESS_RESPONSE//dl-sch
{
    int RA_RNTI;
    int timing_advance;
    int uplink_resource_grant;
    int temporary_c_rnti;
    long checksum;
};

struct DRX_CONFIG
{
    int on_duration_timer;
    int short_drx_timer;
    int long_drx_timer;
    int drx_cycle_type; //0 - short, 1 - long
};

struct DCI_MESSAGE//pdcch
{
    int C_RNTI;
    unsigned char format0_a_flag;
    unsigned char freqency_hooping;
    int riv;
    int mcs;
    unsigned char ndi;
    int tpc;
    int cyclic_shift;
    unsigned char cqi_request;
    struct DRX_CONFIG drx_config;
    long checksum;
};

struct UE_INFO
{
    int UE_state;
    int RNTI;
    int timing_advance;
    int uplink_resource_grant;
    struct DRX_CONFIG uplink_power_control;
    int ul_sch_config;
    int srb_identity;
    clock_t drx_cycle_start;
    int battery_life;
};

struct UPLINK_CONTROL_INFORMATION//pucch
{
    struct UE_INFO ue_info;
    int C_RNTI;
    int scheduling_request;
    int harq_ack;
    int cqi;
};

struct RRC_CONN_REQUEST//ul-sch
{
    int C_RNTI;
    int UE_identity;
    int establishment_cause;
};

struct RRC_CONN_SETUP//dl-sch
{
    int C_RNTI;
    int srb_identity;
    int dl_am_rlc;
    int ul_am_rlc;
    int ul_sch_config;
    int phr_config;
    struct DRX_CONFIG uplink_power_control;
};

struct RRC_CONN_SETUP_COMPLETE//dl-sch
{
    int C_RNTI;
    int PLMN_identity;
    int old_tai;
    int old_gummei;
    int old_guti;
    int ul_sch_config;
    int phr_config;
    int uplink_power_control;
};

#endif
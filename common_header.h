#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

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

struct RANDOM_ACCESS_PREAMBLE
{
    int RA_RNTI;
    short int preamble; //1337
    long checksum;
};

struct RANDOM_ACCESS_RESPONSE
{
    int RA_RNTI;
    int timing_advance;
    int uplink_resource_grant;
    int temporary_c_rnti;
    long checksum;
};

struct DCI_MESSAGE
{
    unsigned char format0_a_flag;
    unsigned char freqency_hooping;
    int riv;
    int mcs;
    unsigned char ndi;
    int tpc;
    int cyclic_shift;
    unsigned char cqi_request;
    long checksum;
};

struct UE_INFO
{
    int UE_state;
    int RNTI;
    int timing_advance;
    int uplink_resource_grant;
    int uplink_power_control;
    int ul_sch_config;
    int srb_identity;
};

struct UPLINK_CONTROL_INFORMATION{
    struct UE_INFO ue_info;
    int C_RNTI;
    int scheduling_request;
    int harq_ack;
    int cqi;
};

struct RRC_CONN_REQUEST
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
    int uplink_power_control;
};

struct CONNECTION_SETUP_COMPLETE{
    int C_RNTI;
};

#endif
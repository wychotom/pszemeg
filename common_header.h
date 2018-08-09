#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

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
    int RA_RNTI;
    int scheduling_request;
    int harq_ack;
    int cqi;
};

#endif
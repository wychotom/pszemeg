#ifndef CHANNELS_STRUCT_H
#define CHANNELS_STRUCT_H

enum Ports
{
    pbch_port = 20700,
    dl_sch_port = 20701,
    ul_sch_port = 20702,
    pdcch_port = 20703,
    pucch_port = 20704,
    prach_port = 20705
};

enum Channel_flags
{
    random_access_response
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

struct RANDOM_ACCESS_RESPONSE_MESSAGE
{
    int ra_rnti;
    int timing_advance;
    int uplink_resource_grant;
    int temporary_c_rnti;
    long checksum;
};

#endif
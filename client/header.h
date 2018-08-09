#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define BROADCAST_PORT 20700

//#define DEBUG 1

// States
// 0 - Before MIB_MESSAGE
// 1 - Sending PRACH
// 2 - awaiting prach response
// 3 - RRC connection request
// 4 - awaiting connection setup
// 5 - send rrc connection complete

struct conn_pair
{
    int sock;
    int port;
};

struct eNB_conn_info
{
    struct conn_pair broadcast;
    struct conn_pair prach;
    struct conn_pair dl_sch;
    struct conn_pair ul_sch;
    struct conn_pair pdcch;
    struct conn_pair pucch;
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
    //int RA_RNTI;//might suggest them this
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

struct UCI_MESSAGE{
    struct UE_INFO info;
    int RA_RNTI;
    int scheduling_request;
    int harq_ack;
    int cqi;
};

//uefuncs.c
int get_unique_name();
void handletraffic();
void setup_ue(struct UE_INFO *);
void init_channel(struct conn_pair *, struct epoll_event *, int *);
void setup_connection_information(struct eNB_conn_info *, struct MIB_MESSAGE);
void open_channels(struct eNB_conn_info *, struct epoll_event *, int *);
void print_cell();

//setup_socket.c
void setup_socket(int *, int);
int set_non_block();
void setup_broadcast_socket();
void add_socket_epoll(struct epoll_event *, int *, int *);

//messages.c
void receive_init_broadcast_msg(struct MIB_MESSAGE *);
void receive_broadcast(int, struct UE_INFO *, struct MIB_MESSAGE *);

void send_random_access_preamble(int, int, struct UE_INFO *);
void receive_random_access_response(int, struct UE_INFO *);

void send_uci(int, int, struct UE_INFO *);
void receive_dci(int, struct UE_INFO *);

#endif
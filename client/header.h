#ifndef HEADER_H
#define HEADER_H

#include "../common_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>

#define printlog(X) _Generic((X),										            \
							struct MIB_MESSAGE: printlog_mib,			            \
							struct DCI_MESSAGE: printlog_dci,			            \
							struct RANDOM_ACCESS_RESPONSE: printlog_rar,            \
							struct RRC_CONN_SETUP: printlog_rrc_setup,	            \
							struct DRX_CONFIG: printlog_drx,			            \
                            struct eNB_conn_info: printlog_drop,		            \
                            struct RANDOM_ACCESS_PREAMBLE: printlog_rap,            \
                            struct UPLINK_CONTROL_INFORMATION: printlog_uci,        \
                            struct RRC_CONN_REQUEST: printlog_rrc_req,              \
                            struct RRC_CONN_SETUP_COMPLETE: printlog_rrc_complete,  \
							default: printlog_default					            \
							)(X)	


//Define DEBUG for messages
//Define INIT_DEBUG for connecting and stuff

// States
// 0 - Before MIB_MESSAGE
// 1 - Sending PRACH
// 2 - awaiting prach response
// 3 - RRC connection request/response
// 4 - connection setup and creating srb socket
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
    struct conn_pair srb;
    struct conn_pair drb;
};


//ue_init.c
int get_unique_name();
void setup_ue(struct UE_INFO *);
void init_channel(struct conn_pair *, struct epoll_event *, int *);
void setup_connection_information(struct eNB_conn_info *, struct MIB_MESSAGE);
void open_channels(struct eNB_conn_info *, struct epoll_event *, int *);

//ue_traffic.c
void handletraffic();
void states_check(struct eNB_conn_info *, struct UE_INFO *);
void handle_drx(struct UE_INFO *, struct conn_pair, time_t *, time_t);

//setup_socket.c
int setup_socket(struct conn_pair *, int);
int set_non_block();
void setup_broadcast_socket();
void add_socket_epoll(struct epoll_event *, int *, int *);

//messages.c
int receive_msg(int, void *, size_t);
int send_msg(struct conn_pair, void *, size_t);

void send_file(struct conn_pair, int *);

void receive_broadcast(int, struct UE_INFO *, struct MIB_MESSAGE *);

void send_random_access_preamble(struct conn_pair, struct UE_INFO *);
void receive_random_access_response(int, struct UE_INFO *);

void send_uci(struct conn_pair, struct UE_INFO *);
void receive_dci(int, struct UE_INFO *);

void send_rrc_req(struct conn_pair, struct UE_INFO *);
void receive_rrc_setup(int, struct UE_INFO *);
void send_rrc_setup_complete(struct conn_pair, struct UE_INFO *);

void drop_packets(struct eNB_conn_info);

//show_cell.c
void print_cell(struct UE_INFO *);
void extract_battery(int, char *);
void print_initial_offset();

//log.c
void printlog_mib(struct MIB_MESSAGE);
void printlog_dci(struct DCI_MESSAGE);
void printlog_rar(struct RANDOM_ACCESS_RESPONSE);
void printlog_rrc_setup(struct RRC_CONN_SETUP);
void printlog_drx(struct DRX_CONFIG);
void printlog_drop(struct eNB_conn_info);
void printlog_default(void);

void printlog_rap(struct RANDOM_ACCESS_PREAMBLE rap_msg);
void printlog_uci(struct UPLINK_CONTROL_INFORMATION uci_msg);
void printlog_rrc_req(struct RRC_CONN_REQUEST rrc_msg);
void printlog_rrc_complete(struct RRC_CONN_SETUP_COMPLETE rrc_msg);
#endif
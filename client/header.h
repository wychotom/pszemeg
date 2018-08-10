#ifndef HEADER_H
#define HEADER_H

#include "../common_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define BROADCAST_PORT 20700
#define BLACK_BG "\e[37m\e[40m"
#define GREEN_BG "\e[37m\e[42m"
#define NORMAL_BG "\e[0m"

//#define DEBUG 1

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
};


//uefuncs.c
int get_unique_name();
void handletraffic();
void setup_ue(struct UE_INFO *);
void init_channel(struct conn_pair *, struct epoll_event *, int *);
void setup_connection_information(struct eNB_conn_info *, struct MIB_MESSAGE);
void open_channels(struct eNB_conn_info *, struct epoll_event *, int *);
void print_cell(struct UE_INFO);
void wait();

void states_check(struct eNB_conn_info *, struct UE_INFO *);

//setup_socket.c
int setup_socket(struct conn_pair *, int);
int set_non_block();
void setup_broadcast_socket();
void add_socket_epoll(struct epoll_event *, int *, int *);

//messages.c
int receive_msg(int, void *, size_t);
int send_msg(struct conn_pair, void *, size_t);

void receive_broadcast(int, struct UE_INFO *, struct MIB_MESSAGE *);

void send_random_access_preamble(struct conn_pair, struct UE_INFO *);
void receive_random_access_response(int, struct UE_INFO *);

void send_uci(struct conn_pair, struct UE_INFO *);
void receive_dci(int, struct UE_INFO *);

void send_rrc_req(struct conn_pair, struct UE_INFO *);
void receive_rrc_setup(int, struct UE_INFO *);
void send_rrc_setup_complete(struct conn_pair, struct UE_INFO *);

#endif
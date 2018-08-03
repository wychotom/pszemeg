#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/utsname.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <time.h>
#include <assert.h>
#include <errno.h>

#define HEADER_LENGTH   6
#define BUFFER_LENGTH   1024
#define BROADCAST_PORT  20700

struct conn_pair
{
    int sock;
    int port;
    int type;//0 downlink 1 uplink
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
    int checksum;
};

struct RANDOM_ACCESS_PREAMBLE
{
    short int preamble; //505245414d424c45
    int RA_RNTI;
};



void open_channels(struct eNB_conn_info * eNB, struct epoll_event *ev, int *efd);
void init_channel(struct conn_pair * channel, struct epoll_event * ev, int * efd);
void add_socket_epoll(struct epoll_event * ev, int * efd, int to_watch);
void handletraffic(struct MIB_MESSAGE *init_msg);

void set_up_socket(int *, int);

int set_non_block();
int get_unique_name();

void receive_broadcast_msg(int *, struct MIB_MESSAGE *);


#endif
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <getopt.h>
#include <assert.h>

#define HEADER_LENGTH   6
#define BUFFER_LENGTH   1024

#define BROADCAST_PORT  20700

//broadcast, shared, control

void send_message(char *, int);
char *receive_message(int);
void handlerecv(int);

void receive_broadcast_msg(int *);
void set_non_block();
const char* getUniqueName();

struct MIB_MESSAGE
{
    int prach_port;
    int broadcast_port;
    int dl_sch_port;
    int ul_sch_port;
    int pdcch_port;
    int pucch_port;
};

#endif
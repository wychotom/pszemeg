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
#define PRACH_PORT      20701
#define DL_SCH_PORT     20702
#define UL_SCH_PORT     20704
#define PDCCH_PORT      20706
#define PUCCH_PORT      20706


//broadcast, shared, control

void send_message(char *, int);
char *receive_message(int);
void handlerecv(int);

void receive_broadcast_msg();
void set_non_block();
const char* getUniqueName();

struct message
{
    char msg_header[HEADER_LENGTH];
    char msg[BUFFER_LENGTH - HEADER_LENGTH];
};

#endif
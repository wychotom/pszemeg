#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */ 

//for Mac OS X
#include <stdlib.h>

#define REMOTE_SERVER_PORT 20700
#define MAX_MSG 100

struct MIB_MESSAGE
{
    int prach_port;
    int broadcast_port;
    int dl_sch_port;
    int ul_sch_port;
    int pdcch_port;
    int pucch_port;
};



int main(int argc, char *argv[]) {
  
  int sd, rc, i;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;
  int broadcast = 1;
  
  /* check command line args */
  if(argc<3) {
    printf("usage : %s <server> <data1> ... <dataN> \n", argv[0]);
    exit(1);
  }

  /* get server IP address (no check if input is IP address or DNS name */
  h = gethostbyname(argv[1]);
  if(h==NULL) {
    printf("%s: unknown host '%s' \n", argv[0], argv[1]);
    exit(1);
  }

  printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
	 inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
	 h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

  /* socket creation */
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }
  
  
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
          perror("setsockopt (SO_BROADCAST)");
          exit(1);
  }
  
  /* bind any port */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: cannot bind port\n", argv[0]);
    exit(1);
  }

  struct MIB_MESSAGE *sending = (struct MIB_MESSAGE *)calloc(sizeof(struct MIB_MESSAGE), 1);

  sending->broadcast_port = 20700;
  sending->prach_port = 20701;
  sending->dl_sch_port = 20702;
  sending->ul_sch_port = 20703;
  sending->pdcch_port = 20704;
  sending->pucch_port = 20705;

    rc = sendto(sd, sending, sizeof(struct MIB_MESSAGE), 0, 
		(struct sockaddr *) &remoteServAddr, 
		sizeof(remoteServAddr));

    if(rc<0) {
      printf("%s: cannot send data %d \n",argv[0],rc);
      close(sd);
      exit(1);
    }

  //}

  return 1;

}
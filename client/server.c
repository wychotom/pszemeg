#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//volatile sig_atomic_t passflag = 0;
#define BUFFER_LENGTH 1024
#define PID_FILE ".pid_log"
#define MAX_CONN 20

void * handleclient(void * vptr_args);
static void pSigHandler(int signo);
void daemonize();
void sendmessage(char *msg, int sock);
char * receivemessage(int sock);

typedef struct {
	char * name;
	int iter;//which socket in acceptSockets
	int socket;
	struct sockaddr_in service;
	int slen;
	//volatile sig_atomic_t doneflag;
} userdata;

typedef struct {
	char * name;
	char msg[BUFFER_LENGTH];
} msginfo;


int mainSocket;
userdata * Userlist = NULL;
//int * acceptSockets = NULL;

int main(int argc, char ** argv)
{

  int quitflag = 0;
  int port = 0;
	int opt;
	opterr = 0;

	while((opt = getopt(argc, argv, "p:q"))!= -1)
	{
		switch(opt)
		{
		case 'p':
			//if(optarg[0] == '0') break;
			port = atoi(optarg);
			break;
		case 'q':
			quitflag = 1;
			break;
		default:
			fprintf(stderr, "BAD BOY\n");
			exit(EXIT_FAILURE);
		}
	}
  if((port && quitflag)||((port == 0)&&(quitflag == 0)))
  {
    printf("Run program with only one argument\nExample:\n"
    "%s -p 20715\n%s -q\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }

  if(quitflag)
  {
    FILE * f = fopen(PID_FILE, "r");
    if(f == NULL)
    {
      fprintf(stdout, "Server is off.\n");
      exit(EXIT_SUCCESS);
    }
    int cpid = 0;
    fscanf(f, "%d", &cpid);
    fclose(f);
    if(cpid != 0)
    {
      kill(cpid, SIGUSR1);
    }
    unlink(PID_FILE);
    exit(EXIT_SUCCESS);
  }
  //printf("%d\n", getpid());

  daemonize();//un/comment to daemonize process

  struct sigaction psa;
  psa.sa_handler = pSigHandler;
  psa.sa_flags = 0;

  sigaction(SIGUSR1, &psa, NULL);
  //sigaction(SIGTSTP, &psa, NULL);

	int bytesSent, bytesRecv;
	char sendbuf[BUFFER_LENGTH];
	char recvbuf[BUFFER_LENGTH];
	memset(&recvbuf,'\0',BUFFER_LENGTH);
	memset(&sendbuf,'\0',BUFFER_LENGTH);

	mainSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == -1)
	{
		printf("error while creating socket.\n");
		return 1;
	}

	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY; //sin_addr struktura zawierajaca s_addr, s_addr moze byc kilka, kilka adresow, niepotrzebne dla adresu loopback
	service.sin_port = htons(port);
	if(bind(mainSocket,(struct sockaddr *) &service,sizeof(struct sockaddr_in)) == -1)
	{
		printf("couldnt bind address to socket. Choose different port\n");
		close(mainSocket);
		return 1;
	}
	if (listen(mainSocket, MAX_CONN) == -1) printf("couldnt listen on socket");

 	Userlist = (userdata *)malloc(sizeof(userdata) * MAX_CONN);
 	pthread_t * tarr = (pthread_t *)malloc(sizeof(pthread_t)* MAX_CONN);
	//memset(&acceptSockets, -1, sizeof(int) * MAX_CONN);
	int accres = 0;
	for(int i = 0; i < MAX_CONN; i++) Userlist[i].socket = -1;
 	while(1)
	{
		//if(acceptSockets[iter%MAX_CONN] == -1) printf("%d Socket", iter%MAX_CONN);
		//iter++;
		for(int i = 0; i < MAX_CONN; i++)
		{
			if(Userlist[i].socket==-1)
			{
				Userlist[i].socket = accept(mainSocket, (struct sockaddr *)&Userlist[i].service, &Userlist[i].slen);
				if(Userlist[i].socket>0)
				{
					Userlist[i].iter = i;
					pthread_create(&tarr[i], 0, &handleclient, &Userlist[i]);
					printf("Podlaczono/\n");
				}
			}
		}
	}
}

void * handleclient(void * vptr_args)
{
	//for(int i = 0; i < MAX_CONN; i++) printf("%s\n",Userlist[i]);
	userdata * args = (userdata *)vptr_args;
	if(fcntl(args->socket, F_GETFL) & O_NONBLOCK)
	{
		if(fcntl(args->socket, F_SETFL, fcntl(args->socket, F_GETFL) | O_NONBLOCK) < 0)
		{
			printf("couldn't set the socket to non-blocking mode");
		}
	}
	char * username = (char *)malloc(sizeof(char) * BUFFER_LENGTH);
	char * hold = NULL;//(char *)malloc(sizeof(char) * BUFFER_LENGTH);
	//memset(users, '\0', BUFFER_LENGTH);
	int size = 0;
	while(hold == NULL || hold == '\0')
	{
		hold = receivemessage(args->socket);
	}
	args->name = hold;
	hold = NULL;

	int flag = 1;
	int foreignsocket[MAX_CONN];
	for(int i = 0; i < MAX_CONN; i++) foreignsocket[i] = -1;
	while(flag)
	{
		sendmessage("connected users, type something if empty", args->socket);
		sleep(2);
		for(int i = 0; i < MAX_CONN; i++)
		{
			if(i == args->iter) continue;
			if(Userlist[i].socket == -1) continue;
			sleep(1);
			sendmessage(Userlist[i].name, args->socket);
		}
		sleep(2);
		while(hold == NULL || hold == '\0')
		{
			hold = receivemessage(args->socket);
		}
		for(int i = 0; i < MAX_CONN; i++)
		{
			if(i == args->iter) continue;
			if(Userlist[i].socket == -1) continue;
			printf("FLAG %s: %s\n", args->name, hold);
			if(strcmp(hold, Userlist[i].name) == 0)
			{
				foreignsocket[i] = Userlist[i].socket;
				flag = 0;
			}
		}
		hold = NULL;
	}
	char messg[BUFFER_LENGTH];
	while(1)
	{
		strcpy(messg, args->name);
		strcat(messg, ": ");
		hold = receivemessage(args->socket);
		if(hold != NULL)
		{
			//printf("%s: %s\n", args->name, hold);
			strcat(messg, hold);
			for(int i = 0; i < MAX_CONN; i++)
			{
				if(foreignsocket[i]!=-1) sendmessage(messg, foreignsocket[i]);
			}

			//printf("xxx%sxxx\n",args->name);
			//printf("xxx%sxxx\n",hold);
			/*if(strcmp(hold, args->name) == 0)
			{
				sendmessage("Shame\n", args->socket);
			}*/
		}
		memset(messg, '\0', BUFFER_LENGTH);

		sleep(1);
	}
}

void sendmessage(char *msg, int sock)
{
	int sentbytes;
	char msgbuff[BUFFER_LENGTH];
	memset(msgbuff, '\0', BUFFER_LENGTH);
	strcpy(msgbuff, msg);//do zmiany
	//sentbytes = sendto(sock, msgbuff, strlen(msgbuff), 0);
	sentbytes = send(sock, msgbuff, strlen(msgbuff) + 1, 0);
}

char * receivemessage(int sock)
{
	int recvbytes;
	char recvbuf[BUFFER_LENGTH];
	memset(recvbuf, '\0', BUFFER_LENGTH);
	recvbytes = recv(sock, recvbuf, BUFFER_LENGTH, MSG_DONTWAIT);
	if(recvbytes > 0)
	{
		if (recvbytes > BUFFER_LENGTH)
		{
			return NULL;
		}
		else
		{
			char * ret = (char *)malloc(sizeof(strlen(recvbuf)) + 1);
			strcpy(ret, recvbuf);
			//ret[strlen(ret)-1] = '\0';
			return ret;
		}
	}
	return NULL;
}

void daemonize()
{
  pid_t pid, sid;
  pid = fork();
  if(pid < 0)
  {
    fprintf(stderr, "couldn't fork.\n");
    exit(EXIT_FAILURE);
  }
  if(pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  pid_t childpid = getpid();
  FILE * f = fopen(PID_FILE, "w");
	fprintf(f, "%d", childpid);
  fclose(f);


  sid = setsid();
  if(sid < 0)
  {
    fprintf(stderr, "couldn't setsid.\n");
    exit(EXIT_FAILURE);
  }
  if(chdir("serverlog") < 0)
  {
    fprintf(stderr, "couldn't open a directory.\n");
    exit(EXIT_FAILURE);
  };

  for(int i = 0; i < getdtablesize(); i++)
  {
    close(i);
  }
  umask(027);
}

static void pSigHandler(int signo)
{
	switch(signo)
	{
		case SIGUSR1:
    //fprintf(stdout, "Kill me.\n");
		//free(acceptSockets);
    exit(EXIT_SUCCESS);
		break;
	}
}

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

#define BUFFER_LENGTH 1024

//using namespace std;
void sendmessage(char *msg, int sock);
char * receivemessage(int sock);
void * handlerecv(void * vptr_args);

char * pmessage = NULL;

int main(int argc, char ** argv)
{

	char * address = NULL;
	char * name = NULL;
	int port = 0;
	int opt;
	opterr = 0;

	while((opt = getopt(argc, argv, "a:p:n:"))!= -1)
	{
		switch(opt)
		{
		case 'p':
			//if(optarg[0] == '0') break;
			port = atoi(optarg);
			break;
		case 'a':
			address = optarg;
			break;
		case 'n':
			name = optarg;
			strcat(name, "\0");
			break;
		default:
			fprintf(stderr, "no known option\n");
			exit(EXIT_FAILURE);
		}
	}
	if((port==0)||(name==NULL)||(address==NULL))
	{
		printf("Run program with three arguments\nExample:\n"
    "%s -p 20715 -n Nagger -a 192.168.0.1\n%s -q\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
	}
	int mainSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == -1)
	{
		printf("error while creating socket.\n");
		return 1;
	}
	char buf[BUFFER_LENGTH];
	struct sockaddr_in sa;
	//struct hostent * he;
	//he = gethostbyname(buf);

	int sh = socket(AF_INET, SOCK_STREAM, 0);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	//memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
	sa.sin_port = htons(port);
	if(connect(sh, (struct sockaddr *)&sa, sizeof(sa))!= -1)
	{
		sendmessage(name, sh);
		sleep(2);
		//char * msgrecv =NULL;
		pthread_t th;
		pthread_create(&th, 0, handlerecv, &sh);
		char buf[BUFFER_LENGTH];
		while(1)
		{
			memset(buf, '\0', BUFFER_LENGTH);
			fgets(buf, BUFFER_LENGTH, stdin);
			buf[strcspn(buf, "\n")] = 0;
			sendmessage(buf, sh);
			//msgrecv = receivemessage(sh);
			//printf("msgrecv = %s", msgrecv);
			sleep(1);
		}
	}
	else
	{
		printf("Server is offline.\n");
	}
	return 0;
}
void sendmessage(char *msg, int sock)
{
	int sentbytes;
	//strcat(msg, "\0");
	char msgbuff[BUFFER_LENGTH];
	memset(msgbuff, '\0', BUFFER_LENGTH);
	strcpy(msgbuff, msg);//do zmiany
	//strcat(msgbuff, "\0");
	sentbytes = send(sock, msgbuff, strlen(msgbuff), 0);
}

char * receivemessage(int sock)
{
	int recvbytes;
	char recvbuf[BUFFER_LENGTH];
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
			ret[strlen(recvbuf) + 1] = '\0';
			return ret;
		}
	}
	if(recvbytes == 0) exit(EXIT_SUCCESS);
	return NULL;
}

void * handlerecv(void * vptr_args)
{
	int sh = *((int *)vptr_args);
	char * result = NULL;
	while(1)
	{
		result = receivemessage(sh);
		if(result != NULL)
		{
			printf("%s\n", result);
		}
	}
}

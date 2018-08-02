#include "header.h"

#include <sys/utsname.h>
#include <time.h>

void send_message(char *msg, int sock)
{
	int sentbytes;
	char msgbuff[BUFFER_LENGTH];
	memset(msgbuff, '\0', BUFFER_LENGTH);
	strcpy(msgbuff, msg);
	sentbytes = send(sock, msgbuff, strlen(msgbuff), 0);
}

char *receive_message(int sock)
{
	int recvbytes;
	char recvbuf[BUFFER_LENGTH];
	struct sockaddr_in clientConfig;

	//recvbytes = recv(sock, recvbuf, BUFFER_LENGTH, MSG_DONTWAIT);
    //recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);
	memset(recvbuf, 0x0, BUFFER_LENGTH);
	int ca_len = sizeof(clientConfig);
    recvbytes = recvfrom(sock, recvbuf, BUFFER_LENGTH, 0, (struct sockaddr *)&clientConfig, &ca_len);
	if(recvbytes > 0)
	{
        printf("yo im here\n");

		if (recvbytes > BUFFER_LENGTH)
		{
			return NULL;
		}
		else
		{
			
		}
	}
	if(recvbytes == 0)
    {
        printf("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
	return NULL;
}

void handlerecv(int socket)
{
	char *result = NULL;
	while(1)
	{
		result = receive_message(socket);
		if(result != NULL)
		{
			printf("%s\n", result);
            free(result);
		}
	}
    return;
}

void receive_broadcast_msg(int * flag)
{
	int broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);

    assert(broadcast_socket != -1);

	int broadcast = 1;
    int ca_len;
	int retval = setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    if(retval == -1)
	{
		perror("Failed on setsockopt\n");
	}

	struct sockaddr_in socketConfig, clientConfig;
    memset(&socketConfig, 0, sizeof(socketConfig));

    socketConfig.sin_family = AF_INET;
	socketConfig.sin_port = htons(BROADCAST_PORT);
	socketConfig.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(broadcast_socket, (struct sockaddr *)&socketConfig, sizeof(struct sockaddr));
    if(retval == -1)
	{
		perror("Failed on bind\n");
	}

	int recvbytes;
	char recvbuf[BUFFER_LENGTH];

	struct MIB_MESSAGE *msg_from_enb = (struct MIB_MESSAGE *)calloc(sizeof(struct MIB_MESSAGE), 1);

	ca_len = sizeof(clientConfig);
    recvbytes = recvfrom(broadcast_socket, msg_from_enb, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);

	if(recvbytes > 0)
	{
		if (recvbytes > BUFFER_LENGTH)
		{
			free(msg_from_enb);
			return;
		}
		else
		{
			*flag = 1;
	    	printf("from ENB %s : UDP %u :\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n"
			,inet_ntoa(clientConfig.sin_addr), ntohs(clientConfig.sin_port), msg_from_enb->broadcast_port,
			msg_from_enb->prach_port, msg_from_enb->dl_sch_port, msg_from_enb->ul_sch_port, msg_from_enb->pdcch_port,
			msg_from_enb->pucch_port);
		}
	}
	if(recvbytes == 0)
    {
        printf("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
	free(msg_from_enb);
    return;
}

const char* getUniqueName()
{

	static struct utsname u;
	if ( uname(&u) < 0 )
	{       
		assert(0);   
		return "unknown";
	}
	return u.nodename;
}
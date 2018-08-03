#include "header.h"

void receive_broadcast_msg(int * flag, struct MIB_MESSAGE * return_MIB)
{
	int broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);

    assert(broadcast_socket != -1);

	int broadcast = 1;
    unsigned int ca_len;
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
			*return_MIB = *msg_from_enb;
		}
	}
	if(recvbytes == 0)
    {
        perror("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
	if(!flag)
		free(msg_from_enb);

    return;
}

int get_unique_name()
{
	srand(time(NULL));
	return rand() % RAND_MAX;
}

void set_up_socket(int * sockfd, int port)
{
	int errflag;

	printf("SUS port = %d\n", port);

	struct sockaddr_in sa;

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);

	errflag = connect(*sockfd, (struct sockaddr *)&sa, sizeof(sa));

	if (errflag == -1)
	{
		if(errno == EINPROGRESS)
		{

		}
		else
			perror("connect err");
		//return -1;
	}
	
}

int set_non_block(int sockfd)
{
	int flags, retval;

	flags = fcntl (sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	retval = fcntl (sockfd, F_SETFL, flags);
	if (retval == -1)
	{
		perror ("fcntl");
		return -1;
	}
	return 0;
}

void open_channels(struct eNB_conn_info * eNB, struct epoll_event *ev, int *efd)
{
	//printf("broadcast\n");
	//init_channel(&eNB->broadcast, ev, efd);	

	printf("prach\n");
	printf("OC: sock = %d\t port = %d\n", eNB->prach.sock ,eNB->prach.port);
	init_channel(&eNB->prach, ev, efd);
	printf("wot\n");
	

	printf("dl_sch\n");
	init_channel(&eNB->dl_sch, ev, efd);

	printf("ul_sch\n");
	init_channel(&eNB->ul_sch, ev, efd);

	printf("pdcch\n");
	init_channel(&eNB->pdcch, ev, efd);

	printf("pucch\n");
	init_channel(&eNB->pucch, ev, efd);
}

void init_channel(struct conn_pair *channel, struct epoll_event *ev, int *efd)
{
	printf("SUS: sock = %d\t port = %d\n", channel->sock ,channel->port);
	set_up_socket(&channel->sock, channel->port);
	set_non_block(channel->sock);
	add_socket_epoll(ev, efd, (*channel).sock);

}

void add_socket_epoll(struct epoll_event *ev, int *efd, int to_watch)
{
	(*ev).events = EPOLLIN;
	(*ev).data.fd = to_watch;
	if (epoll_ctl(*efd, EPOLL_CTL_ADD, to_watch, ev) == -1) {
		perror("epoll_ctl: xd");
		exit(EXIT_FAILURE);
	}
}

void handletraffic(struct MIB_MESSAGE * init_msg)
{
	struct eNB_conn_info connection_information;//Ideally it would be an array, information about multiple eNB
	printf("HT msg: sock = %d\t port = %d\n", 0, init_msg->broadcast_port);

	connection_information.broadcast.port = init_msg->broadcast_port;
	connection_information.broadcast.sock = 0;
	connection_information.broadcast.type = 0;

	connection_information.prach.port = init_msg->prach_port;
	connection_information.prach.sock = 0;
	connection_information.prach.type = 1;

	connection_information.dl_sch.port = init_msg->dl_sch_port;
	connection_information.dl_sch.sock = 0;
	connection_information.dl_sch.type = 0;

	connection_information.ul_sch.port = init_msg->ul_sch_port;
	connection_information.ul_sch.port = 0;
	connection_information.ul_sch.type = 1;

	connection_information.pdcch.port = init_msg->pdcch_port;
	connection_information.pdcch.sock = 0;
	connection_information.pdcch.type = 0;
	
	connection_information.pucch.port = init_msg->pucch_port;
	connection_information.pucch.sock = 0;
	connection_information.pucch.type = 1;

	int efd;
	const int max_epoll_events = 6;
	struct epoll_event ev, events[max_epoll_events];

	printf("HT: sock = %d\t port = %d\n", connection_information.prach.sock, connection_information.prach.port);
	open_channels(&connection_information, &ev, &efd);
}
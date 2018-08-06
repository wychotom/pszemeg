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
	//struct MIB_MESSAGE *msg_from_enb = (struct MIB_MESSAGE *)calloc(sizeof(struct MIB_MESSAGE), 1);

	ca_len = sizeof(clientConfig);
    recvbytes = recvfrom(broadcast_socket, return_MIB, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);

	if(recvbytes > 0)
	{
		if (recvbytes > BUFFER_LENGTH)
		{
			//free(msg_from_enb);
			//shutdown(broadcast_socket, SHUT_WR);
			return;
		}
		else
		{
			*flag = broadcast_socket;
			//*return_MIB = *msg_from_enb;
			return;
		}
	}
	if(recvbytes == 0)
    {
        perror("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
	//if(!flag)
	//	free(msg_from_enb);

	//shutdown(broadcast_socket, SHUT_WR);
	//free(msg_from_enb);
    return;
}

int get_unique_name()
{
	srand(time(NULL));
	return rand() % 10000000 + 10000000;
}

void set_up_socket(int * sockfd, int port)
{
	int errflag;

	//printf("SUS port = %d\n", port);

	struct sockaddr_in sa;

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &errflag, sizeof(errflag));
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
	printf("broadcast\n");
	//init_channel(&eNB->broadcast, ev, efd);	
	add_socket_epoll(ev, efd, &eNB->broadcast.sock);

	printf("pdcch\n");
	//printf("OC: sock = %d\t port = %d\n", eNB->prach.sock ,eNB->prach.port);
	init_channel(&eNB->pdcch, ev, efd);


	printf("prach\n");
	// //printf("OC: sock = %d\t port = %d\n", eNB->prach.sock ,eNB->prach.port);
	init_channel(&eNB->prach, ev, efd);
	// //printf("wot\n");
	

	printf("dl_sch\n");
	init_channel(&eNB->dl_sch, ev, efd);

	printf("ul_sch\n");
	init_channel(&eNB->ul_sch, ev, efd);

	
	printf("pucch\n");
	init_channel(&eNB->pucch, ev, efd);
}

void init_channel(struct conn_pair *channel, struct epoll_event *ev, int *efd)
{
	//printf("SUS: sock = %d\t port = %d\n", channel->sock ,channel->port);
	set_up_socket(&channel->sock, channel->port);
	set_non_block(channel->sock);
	add_socket_epoll(ev, efd, &channel->sock);

}

void add_socket_epoll(struct epoll_event *ev, int *efd, int *to_watch)
{

	ev->events = EPOLLIN;
	ev->data.fd = *to_watch;
	if (epoll_ctl(*efd, EPOLL_CTL_ADD, *to_watch, ev) == -1) {
		perror("epoll_ctl: xd");
		exit(EXIT_FAILURE);
	}
}

void handletraffic(struct MIB_MESSAGE * init_msg, int broadcast_sock)
{
	struct eNB_conn_info connection_information = //Ideally it would be an array, information about multiple eNB
	{
		.broadcast.port = init_msg->broadcast_port,
		.broadcast.sock = broadcast_sock,
		//connection_information.broadcast.type = 0;

		.prach.port = init_msg->prach_port,
		.prach.sock = 0,
		//connection_information.prach.type = 1;

		.dl_sch.port = init_msg->dl_sch_port,
		.dl_sch.sock = 0,
		//connection_information.dl_sch.type = 0;

		.ul_sch.port = init_msg->ul_sch_port,
		.ul_sch.port = 0,
		//connection_information.ul_sch.type = 1;

		.pdcch.port = init_msg->pdcch_port,
		.pdcch.sock = 0,
		//connection_information.pdcch.type = 0;
		
		.pucch.port = init_msg->pucch_port,
		.pucch.sock = 0
	};

	// connection_information.broadcast.port = init_msg->broadcast_port;
	// connection_information.broadcast.sock = broadcast_sock;
	// //connection_information.broadcast.type = 0;

	// connection_information.prach.port = init_msg->prach_port;
	// connection_information.prach.sock = 0;
	// //connection_information.prach.type = 1;

	// connection_information.dl_sch.port = init_msg->dl_sch_port;
	// connection_information.dl_sch.sock = 0;
	// //connection_information.dl_sch.type = 0;

	// connection_information.ul_sch.port = init_msg->ul_sch_port;
	// connection_information.ul_sch.port = 0;
	// //connection_information.ul_sch.type = 1;

	// connection_information.pdcch.port = init_msg->pdcch_port;
	// connection_information.pdcch.sock = 0;
	// //connection_information.pdcch.type = 0;
	
	// connection_information.pucch.port = init_msg->pucch_port;
	// connection_information.pucch.sock = 0;
	//connection_information.pucch.type = 1;
	//free(init_msg);

	printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
			connection_information.broadcast.port, init_msg->prach_port, init_msg->dl_sch_port,
			init_msg->ul_sch_port, init_msg->pdcch_port, init_msg->pucch_port);

	int efd;
	const int max_epoll_events = 6;
	struct epoll_event ev, events[max_epoll_events];

	//printf("HT: sock = %d\t port = %d\n", connection_information.prach.sock, connection_information.prach.port);
	efd = epoll_create(1);

	//int listen_socket;
	//add_socket_epoll(&ev, &efd, &listen_socket);

	//ev.events = EPOLL
	open_channels(&connection_information, &ev, &efd);

	int ewait_flag, i;

	while(1)
	{
		ewait_flag = epoll_wait(efd, events, max_epoll_events, -1);
		if(ewait_flag == -1)
		{
			perror("epoll wait ");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < ewait_flag; i++)
		{
			if(events[i].events & EPOLLOUT)
			{
				
			}

			if(events[i].events & EPOLLIN)
			{
				if(events[i].data.fd == connection_information.pdcch.sock)
				{
					//struct DCI_MESSAGE *killme= (struct DCI_MESSAGE *)calloc(sizeof(struct DCI_MESSAGE), 1);
					struct DCI_MESSAGE killme;

					struct sockaddr_in clientConfig;
					int recvbytes;

					unsigned int ca_len = sizeof(clientConfig);
					recvbytes = recvfrom(events[i].data.fd, &killme, sizeof(struct DCI_MESSAGE), 0,
										(struct sockaddr *)&clientConfig, &ca_len);

					int calc_check_sum = 0;

					calc_check_sum += killme.format0_a_flag;
					calc_check_sum += killme.freqency_hooping;
					calc_check_sum += killme.riv;
					calc_check_sum += killme.mcs;
					calc_check_sum += killme.ndi;
					calc_check_sum += killme.tpc;
					calc_check_sum += killme.cyclic_shift;
					calc_check_sum += killme.cqi_request;//tbi

					if(recvbytes > 0)
					{
						if (recvbytes > BUFFER_LENGTH)
						{
							//free(killme);
							return;
						}
						else
						{
							// printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
							// 		"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
							// 		killme->format0_a_flag, killme->freqency_hooping, killme->riv, killme->mcs,
							// 		killme->ndi, killme->tpc, killme->cyclic_shift, killme->cqi_request);
							printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
							 		"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
							 		killme.format0_a_flag, killme.freqency_hooping, killme.riv, killme.mcs,
							 		killme.ndi, killme.tpc, killme.cyclic_shift, killme.cqi_request);
						}
					}
					//free(killme);
				}
				if(events[i].data.fd == connection_information.broadcast.sock)
				{
					//struct MIB_MESSAGE *killme= (struct MIB_MESSAGE *)calloc(sizeof(struct DCI_MESSAGE), 1);
					struct MIB_MESSAGE killme;
					struct sockaddr_in clientConfig;
					int recvbytes;

					unsigned int ca_len = sizeof(clientConfig);
					recvbytes = recvfrom(events[i].data.fd, &killme, sizeof(struct MIB_MESSAGE), 0,
										(struct sockaddr *)&clientConfig, &ca_len);

					if(recvbytes > 0)
					{
						if (recvbytes > BUFFER_LENGTH)
						{
							//free(msg_from_enb);
							//free(killme);
							return;
						}
						else
						{
							// printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
							// 		init_msg->broadcast_port, init_msg->prach_port, init_msg->dl_sch_port,
							// 		init_msg->ul_sch_port, init_msg->pdcch_port, init_msg->pucch_port);
							printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
									killme.broadcast_port, killme.prach_port, killme.dl_sch_port,
									killme.ul_sch_port, killme.pdcch_port, killme.pucch_port);
						}
					}
					//free(killme);
				}
				
			}
		}
	}
}
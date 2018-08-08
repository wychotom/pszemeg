#include "header.h"

int get_unique_name()
{
	srand(time(NULL));
	return rand() % 10000000 + 10000000;
}

void set_up_socket(int * sockfd, int port)
{
	int errflag = 1;

	struct sockaddr_in sa;

	*sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &errflag, sizeof(errflag));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);

	//errflag = connect(*sockfd, (struct sockaddr *)&sa, sizeof(sa)); // TU COS ZMIENILEM NIE WIEM CO XD
    errflag = bind(*sockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr));

	if (errflag == -1)
	{
		if(errno == EINPROGRESS)
		{
			
		}
		else
			perror("connect err");
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
	add_socket_epoll(ev, efd, &eNB->broadcast.sock);

	printf("pdcch\n");
	init_channel(&eNB->pdcch, ev, efd);


	printf("prach\n");
	init_channel(&eNB->prach, ev, efd);
	

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

void handletraffic(struct MIB_MESSAGE *init_msg, int broadcast_sock)
{
	struct eNB_conn_info connection_information = //Ideally it would be an array, information about multiple eNB
	{
		.broadcast.port = init_msg->broadcast_port,
		.broadcast.sock = broadcast_sock,

		.prach.port = init_msg->prach_port,
		.prach.sock = 0,

		.dl_sch.port = init_msg->dl_sch_port,
		.dl_sch.sock = 0,

		.ul_sch.port = init_msg->ul_sch_port,
		.ul_sch.port = 0,

		.pdcch.port = init_msg->pdcch_port,
		.pdcch.sock = 0,
		
		.pucch.port = init_msg->pucch_port,
		.pucch.sock = 0
	};

	printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
			connection_information.broadcast.port, init_msg->prach_port, init_msg->dl_sch_port,
			init_msg->ul_sch_port, init_msg->pdcch_port, init_msg->pucch_port);

	struct UE_INFO my_states;

	setup_ue(&my_states);

	int efd;
	const int max_epoll_events = 6;
	struct epoll_event ev, events[max_epoll_events];

	efd = epoll_create(1);

	open_channels(&connection_information, &ev, &efd);

	int ewait_flag, i;

	while(1)
	{
		if(my_states.UE_state == 1)
		{
			send_random_access_preamble(connection_information.prach.sock, &my_states);
		}

		send_uci(connection_information.pucch.sock, &my_states);

		ewait_flag = epoll_wait(efd, events, max_epoll_events, -1);
		if(ewait_flag == -1)
		{
			perror("epoll wait ");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < ewait_flag; i++)
		{
			if(events[i].events & EPOLLIN)
			{
				if(events[i].data.fd == connection_information.pdcch.sock)
				{
					receive_dci(events[i].data.fd, &my_states);
				}
				if(events[i].data.fd == connection_information.broadcast.sock)
				{
					receive_broadcast(events[i].data.fd, &my_states);
				}

				if((events[i].data.fd == connection_information.dl_sch.sock) 
					&& (my_states.UE_state == 1))
				{
					printf("I TRIED SO HARD AND GOT SO FAR\nBUT IN THE END IT JUST END UP BLOCKED\n");
					receive_random_access_response(events[i].data.fd, &my_states);
				}
				
			}
		}
	}
}

void setup_ue(struct UE_INFO *init_states)
{
	int RNTI = get_unique_name();

	printf("Uniq name = %d\n", RNTI);

	init_states->UE_state = 1;
	init_states->RNTI = RNTI;
	init_states->timing_advance = 0;
	init_states->uplink_resource_grant = 0;
	init_states->uplink_power_control = 0;
	init_states->ul_sch_config = 0;
	init_states->srb_identity = 0;
}
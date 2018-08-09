#include "header.h"

#define Nop() asm (" nop ");

#include <time.h>
#include <sys/socket.h>
#include <errno.h>

int get_unique_name()
{
	srand(time(NULL));
	return rand() % 10000000 + 10000000;
}



void open_channels(struct eNB_conn_info * eNB, struct epoll_event *ev, int *efd)
{
	#ifdef DEBUG
	printf("broadcast\n");
	#endif
	add_socket_epoll(ev, efd, &eNB->broadcast.sock);

	#ifdef DEBUG
	printf("pdcch\n");
	#endif
	init_channel(&eNB->pdcch, ev, efd);

	#ifdef DEBUG
	printf("prach\n");
	#endif
	init_channel(&eNB->prach, ev, efd);
	
	#ifdef DEBUG
	printf("dl_sch\n");
	#endif
	init_channel(&eNB->dl_sch, ev, efd);

	#ifdef DEBUG
	printf("ul_sch\n");
	#endif
	init_channel(&eNB->ul_sch, ev, efd);
	
	#ifdef DEBUG
	printf("pucch\n");
	#endif
	init_channel(&eNB->pucch, ev, efd);
}

void init_channel(struct conn_pair *channel, struct epoll_event *ev, int *efd)
{
	setup_socket(&channel->sock, channel->port);
	set_non_block(channel->sock);
	add_socket_epoll(ev, efd, &channel->sock);
}

void handletraffic()
{
	struct eNB_conn_info connection_information = {};//Ideally it would be an array, information about multiple eNB

	struct UE_INFO my_states;
	struct MIB_MESSAGE init_mib_msg;

	setup_ue(&my_states);

	int broadcast_sock;
	setup_broadcast_socket(&broadcast_sock);
	while(my_states.UE_state == 0)
	{
		receive_broadcast(broadcast_sock, &my_states, &init_mib_msg);
	}
	setup_connection_information(&connection_information, init_mib_msg);
	connection_information.broadcast.sock = broadcast_sock;

	int efd;
	const int max_epoll_events = 6;
	struct epoll_event ev, events[max_epoll_events];

	efd = epoll_create(1);

	open_channels(&connection_information, &ev, &efd);

	int ewait_flag, i;

	while(1)
	{
		ewait_flag = epoll_wait(efd, events, max_epoll_events, -1);

        if(my_states.UE_state == 1)
        {
            send_random_access_preamble(connection_information.prach.sock, connection_information.prach.port,&my_states);
        }

        send_uci(connection_information.pucch.sock, connection_information.pucch.port, &my_states);

		if(ewait_flag == -1)
		{
			perror("epoll wait ");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < ewait_flag; i++)
		{
			if(events[i].events & EPOLLIN)
			{
				if(my_states.UE_state == 1)
				{
					send_random_access_preamble(connection_information.prach.sock, 
						connection_information.prach.port,&my_states);
						for(int i = 0; i < 1000000; i++)
							Nop();
				}

				send_uci(connection_information.pucch.sock,
					connection_information.pucch.port, &my_states);

				if(events[i].data.fd == connection_information.pdcch.sock)
				{
					receive_dci(events[i].data.fd, &my_states);
				}
				if(events[i].data.fd == connection_information.broadcast.sock)
				{
					receive_broadcast(events[i].data.fd, &my_states, &init_mib_msg);
				}

				if((events[i].data.fd == connection_information.dl_sch.sock) 
					&& (my_states.UE_state == 1))
				{
					#ifdef DEBUG
					printf("receive_random_access_response\n");
					#endif
					receive_random_access_response(events[i].data.fd, &my_states);
				}
			}
		}
	}
}

void setup_ue(struct UE_INFO *init_states)
{
	int RNTI = get_unique_name();

	#ifdef DEBUG
	printf("Uniq name = %d\n", RNTI);
	#endif
	
	init_states->UE_state = 0;
	init_states->RNTI = RNTI;
	init_states->timing_advance = 0;
	init_states->uplink_resource_grant = 0;
	init_states->uplink_power_control = 0;
	init_states->ul_sch_config = 0;
	init_states->srb_identity = 0;
}

void setup_connection_information(struct eNB_conn_info *conn_info, struct MIB_MESSAGE init_msg)
{
	conn_info->broadcast.port = init_msg.broadcast_port;
	conn_info->broadcast.sock = 0;

	conn_info->prach.port = init_msg.prach_port;
	conn_info->prach.sock = 0;

	conn_info->dl_sch.port = init_msg.dl_sch_port;
	conn_info->dl_sch.sock = 0;

	conn_info->ul_sch.port = init_msg.ul_sch_port;
	conn_info->ul_sch.port = 0;

	conn_info->pdcch.port = init_msg.pdcch_port;
	conn_info->pdcch.sock = 0;
	
	conn_info->pucch.port = init_msg.pucch_port;
	conn_info->pucch.sock = 0;
}

void print_cell()
{
	printf("	                  .--.\n"
	"                          |  |\n"
	"                          |  |\n"
	"                          |  |\n"
	"                          |  |\n"
	"         _.-----------._  |  |\n"
	"      .-'      __       `-.  |\n"
	"    .'       .'  `.        `.|\n"
	"   ;         :    :          ;\n"
	"   |         `.__.'          |\n"
	"   |   ___                   |\n"
	"   |  (_E_) E R I C S S O N  |\n"
	"   | .---------------------. |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | |                     | |\n"
	"   | `---------------------' |\n"
	"   |                         |\n"
	"   |                __       |\n"
	"   |  ________  .-~~__~~-.   |\n"
	"   | |___C___/ /  .'  `.  \\  |\n"
	"   |  ______  ;   : OK :   ; |\n"
	"   | |__A___| |  _`.__.'_  | |\n"
	"   |  _______ ; \\< |  | >/ ; |\n");
}
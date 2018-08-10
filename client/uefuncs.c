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
	setup_socket(channel, SOCK_DGRAM);
	set_non_block(channel->sock);
	add_socket_epoll(ev, efd, &channel->sock);
}

void handletraffic()
{
	struct eNB_conn_info connection_information;//Ideally it would be an array, information about multiple eNB

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

		states_check(&connection_information, &my_states);

        send_uci(connection_information.pucch, &my_states);

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
					#ifdef DEBUG
					printf("receive_dci\n");
					#endif
					receive_dci(events[i].data.fd, &my_states);
				}
				if(events[i].data.fd == connection_information.broadcast.sock)
				{
					#ifdef DEBUG
					printf("receive_broadcast\n");
					#endif
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

				if((events[i].data.fd == connection_information.dl_sch.sock) 
					&& (my_states.UE_state == 3))
				{
					#ifdef DEBUG
					printf("receive_rrc_setup\n");
					#endif
					receive_rrc_setup(events[i].data.fd, &my_states);
				}
			}
		}
		#ifndef DEBUG
		print_cell(my_states);
		#endif
		wait();
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
	conn_info->ul_sch.sock = 0;

	conn_info->pdcch.port = init_msg.pdcch_port;
	conn_info->pdcch.sock = 0;
	
	conn_info->pucch.port = init_msg.pucch_port;
	conn_info->pucch.sock = 0;

	conn_info->srb.port = 0;
	conn_info->srb.sock = 0;
}

void states_check(struct eNB_conn_info *connections, struct UE_INFO *info)
{
	if(info->UE_state == 1)
	{
		#ifdef DEBUG
		printf("send_random_access_preamble\n");
		#endif
		send_random_access_preamble(connections->prach, info);
	}

	if(info->UE_state == 2)
	{
		#ifdef DEBUG
		printf("send_rrc_req\n");
		#endif
		send_rrc_req(connections->ul_sch, info);
	}

	if(info->UE_state == 4)
	{
		connections->srb.port = info->srb_identity;
		if(setup_socket(&connections->srb, SOCK_STREAM) == -1)
		{
			printf("Error on setup socket\n");
			exit(EXIT_FAILURE);
		}
		info->UE_state = 5;
	}

	if(info->UE_state == 5)
	{
		send_rrc_setup_complete(connections->srb, info);
	}
}

void print_cell(struct UE_INFO state)
{
	const int downward_arrow_pos = 21;
	const int upward_arrow_pos = 22;
	const int percent = 23;//23 24 25

	const char cellphoneup[] = //I HOPE ITS BEAUTIFUL
	"	                  "BLACK_BG".--."NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"         "BLACK_BG"_.-----------._  |  |"NORMAL_BG"\n"
	"      "BLACK_BG".-'      __       `-.  |"NORMAL_BG"\n"
	"    "BLACK_BG".'       .'  `.        `.|"NORMAL_BG"\n"
	"   "BLACK_BG";         :    :          ;"NORMAL_BG"\n"
	"   "BLACK_BG"|         `.__.'          |"NORMAL_BG"\n"
	"   "BLACK_BG"|   ___                   |"NORMAL_BG"\n"
	"   "BLACK_BG"|  (_E_) E R I C S S O N  |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG".---------------------."BLACK_BG" |"NORMAL_BG"\n";
	char cellphonedata[] = "   "BLACK_BG"| "GREEN_BG"|               ⇩⬆99%%|"BLACK_BG" |"NORMAL_BG"\n";//	"   | |                     | |\n" //▲△✈ ⬇⇩ ⇧⬆
	const char cellphonedown[] = "   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"`---------------------'"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"|                         |"NORMAL_BG"\n"
	"   "BLACK_BG"|                __       |"NORMAL_BG"\n"
	"   "BLACK_BG"|  ________  .-~~__~~-.   |"NORMAL_BG"\n"
	"   "BLACK_BG"| |___C___/ /  .'  `.  \\  |"NORMAL_BG"\n"
	"   "BLACK_BG"|  ______  ;   : OK :   ; |"NORMAL_BG"\n"
	"   "BLACK_BG"| |__A___| |  _`.__.'_  | |"NORMAL_BG"\n"
	"   "BLACK_BG"|  _______ ; \\< |  | >/ ; |"NORMAL_BG"\n";

	printf("\e[s%s%s%s\e[u", cellphoneup, cellphonedata, cellphonedown);
}

inline void wait()
{
	for(int i = 0; i < 1000000; i++)
		Nop();
}
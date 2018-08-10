#include "header.h"

#include "time.h"

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
	//init_states->uplink_power_control = {};
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

int get_unique_name()
{
	srand(time(NULL));
	return rand() % 10000000 + 10000000;
}
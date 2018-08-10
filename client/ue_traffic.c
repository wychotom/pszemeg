#include "header.h"

#define Nop() asm (" nop ");

#include <time.h>
#include <sys/socket.h>
#include <wchar.h>

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

	struct timespec start, check;

	clock_gettime(CLOCK_REALTIME, &start);

	#ifndef DEBUG
		for(i = 0; i < 29; i++) // TRZEBA BEDZIE ZAMIENIC XD
			printf("\n");
	#endif

	int in_drx = 0;

	while(1)
	{
		#ifdef DEBUG
			clock_gettime(CLOCK_REALTIME, &check);
			printf("TIME = %ld s\n", check.tv_sec - start.tv_sec);
		#endif
<<<<<<< HEAD
		if((((check.tv_sec - start.tv_sec) % my_states.uplink_power_control.short_drx_timer) == 0)
				&& (check.tv_sec - start.tv_sec) != 0)
		{
			in_drx = !in_drx;
		}

		// if((((check.tv_sec - start.tv_sec) % 10) == 0)
		// 		&& (check.tv_sec - start.tv_sec) != 0)
		// {
		// 	in_drx = !in_drx;
		// 	if(my_states.battery_life >= 10)
		// 		my_states.battery_life -= 10;
		// }
	
=======
		// if((((check.tv_sec - start.tv_sec) % my_states.uplink_power_control.short_drx_timer) == 0)
		// 		&& (check.tv_sec - start.tv_sec) != 0)
		// {
		// 	in_drx = !in_drx;
		// }

		if((((check.tv_sec - start.tv_sec) % 10) == 0)
				&& (check.tv_sec - start.tv_sec) != 0)
		{
			in_drx = !in_drx;
			if(my_states.battery_life >= 10)
				my_states.battery_life -= 10;
		}

>>>>>>> 5d65119037139208b9b112df5bb64e62f285530a
		ewait_flag = epoll_wait(efd, events, max_epoll_events, -1);
		
		if(!in_drx)
		{
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
							printf("\nreceive_dci\n");
						#endif
						receive_dci(events[i].data.fd, &my_states);
					}
					if(events[i].data.fd == connection_information.broadcast.sock)
					{
						#ifdef DEBUG
							printf("\nreceive_broadcast\n");
						#endif
						receive_broadcast(events[i].data.fd, &my_states, &init_mib_msg);
					}

					if((events[i].data.fd == connection_information.dl_sch.sock) 
						&& (my_states.UE_state == 1))
					{
						#ifdef DEBUG
							printf("\nreceive_random_access_response\n");
						#endif
						receive_random_access_response(events[i].data.fd, &my_states);
					}

					if((events[i].data.fd == connection_information.dl_sch.sock) 
						&& (my_states.UE_state == 3))
					{
						#ifdef DEBUG
							printf("\nreceive_rrc_setup\n");
						#endif
						receive_rrc_setup(events[i].data.fd, &my_states);
					}
				}
			}
		}
		else
		{
			drop_packets(connection_information);
		}
		
		#ifndef DEBUG
			print_cell(my_states);
		#endif
		//wait();
	}
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
			#ifdef DEBUG
				printf("Error on setup socket\n");
			#endif
			exit(EXIT_FAILURE);
		}
		info->UE_state = 5;
	}

	if(info->UE_state == 5)
	{
		send_rrc_setup_complete(connections->srb, info);
	}
}

void drop_packets(struct eNB_conn_info connections)
{
	receive_msg(connections.broadcast.sock, NULL, sizeof(struct MIB_MESSAGE));
	receive_msg(connections.dl_sch.sock, NULL, sizeof(struct MIB_MESSAGE));
	receive_msg(connections.pdcch.sock, NULL, sizeof(struct MIB_MESSAGE));
}


inline void wait()
{
	for(int i = 0; i < 1000000; i++)
		Nop();
}
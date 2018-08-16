#include "header.h"
#include <string.h>

void receive_broadcast(int fd, struct UE_INFO *info, struct MIB_MESSAGE *mib_ret)
{
	struct MIB_MESSAGE mib_msg;
	int retval = receive_msg(fd, &mib_msg, sizeof(struct MIB_MESSAGE));
	
	if(retval == 0)
	{
		if(info->UE_state == INIT_BROADCAST)
			info->UE_state = RANDOM_ACCESS_PREAMBLE;

		mib_ret->broadcast_port = mib_msg.broadcast_port;
		mib_ret->prach_port = mib_msg.prach_port;
		mib_ret->dl_sch_port = mib_msg.dl_sch_port;
		mib_ret->ul_sch_port = mib_msg.ul_sch_port;
		mib_ret->pdcch_port = mib_msg.pdcch_port;
		mib_ret->pucch_port = mib_msg.pucch_port;
		#ifdef DEBUG
			printlog(mib_msg);
		#endif
	}
}

void receive_dci(int fd, struct UE_INFO *info)
{
	struct DCI_MESSAGE dci_msg;
	int retval = receive_msg(fd, &dci_msg, sizeof(struct DCI_MESSAGE));
	if(retval == 0)
	{
		#ifdef DEBUG
			printlog(dci_msg);
		#endif
		info->uplink_power_control = dci_msg.drx_config;
	}
}

void send_random_access_preamble(struct conn_pair connection, struct UE_INFO *info)
{
    struct RANDOM_ACCESS_PREAMBLE rap_msg;
    const short int preamble_identifier = 1337;

    rap_msg.RA_RNTI = info->RNTI;
    rap_msg.preamble = preamble_identifier;
	
	int retval = send_msg(connection, &rap_msg, sizeof(struct RANDOM_ACCESS_PREAMBLE));

	if(retval == 0)
	{
		#ifdef DEBUG
			printlog(rap_msg);
		#endif
	    info->UE_state = RANDOM_ACCESS_PREAMBLE; // sending rap all the time
	}
}

void receive_random_access_response(int fd, struct UE_INFO *info)
{
	struct RANDOM_ACCESS_RESPONSE rar_msg;

	int retval = receive_msg(fd, &rar_msg, sizeof(struct RANDOM_ACCESS_RESPONSE));
	if(retval == 0)
	{
		if(rar_msg.RA_RNTI == info->RNTI)
			info->RNTI = rar_msg.temporary_c_rnti;

		#ifdef DEBUG
			printlog(rar_msg);
		#endif
		info->UE_state = RANDOM_ACCESS_RESPONSE;
	}
}

void send_uci(struct conn_pair connection, struct UE_INFO *info)
{
	struct UPLINK_CONTROL_INFORMATION uci_msg;

	uci_msg.ue_info = *info;
	uci_msg.C_RNTI = info->RNTI;
	uci_msg.scheduling_request = 1;
	uci_msg.harq_ack = 1;
	uci_msg.cqi = 1;

	if(send_msg(connection, &uci_msg, sizeof(struct UPLINK_CONTROL_INFORMATION)) == 0)
	{
		#ifdef DEBUG
			printlog(uci_msg);
		#endif
	}
}

void send_rrc_req(struct conn_pair connection, struct UE_INFO *info)
{
	struct RRC_CONN_REQUEST rrc_msg;

	enum establishment_causes cause = mobile_originating_signaling;

	rrc_msg.C_RNTI = info->RNTI;
	rrc_msg.UE_identity = 3;//should be random from 0 to 2^40
	rrc_msg.establishment_cause = cause;

	int retval = send_msg(connection, &rrc_msg, sizeof(struct RRC_CONN_REQUEST));
	if(retval == 0)
	{
		#ifdef DEBUG
			printlog(rrc_msg);
		#endif
		info->UE_state = RRC_REQUEST;
	}
}

void receive_rrc_setup(int fd, struct UE_INFO *info)
{
	struct RRC_CONN_SETUP rrc_msg;

	int retval = receive_msg(fd, &rrc_msg, sizeof(struct RRC_CONN_SETUP));

	if(retval == 0)
	{
		if(rrc_msg.C_RNTI == info->RNTI)
		{
			info->srb_identity = rrc_msg.srb_identity;
			info->drb_identity = rrc_msg.drb_identity;

			info->uplink_power_control = rrc_msg.uplink_power_control;

			info->ul_sch_config = rrc_msg.ul_sch_config;
			info->UE_state = RRC_SETUP;
			
			#ifdef DEBUG
				printlog(rrc_msg);
			#endif
		}
	}
}

void send_rrc_setup_complete(struct conn_pair connection, struct UE_INFO *info)
{
	struct RRC_CONN_SETUP_COMPLETE rrc_msg;

	rrc_msg.C_RNTI = info->RNTI;
    rrc_msg.PLMN_identity = 2137;
    rrc_msg.old_tai = 1337;
    rrc_msg.old_gummei = 14;
    rrc_msg.old_guti = 88;
    rrc_msg.ul_sch_config = 15;
    rrc_msg.phr_config = 16;
    rrc_msg.uplink_power_control = 17;

	int retval = send_msg(connection, &rrc_msg, sizeof(struct RRC_CONN_SETUP_COMPLETE));
	if(retval == 0)
	{
		#ifdef DEBUG
				printlog(rrc_msg);
		#endif
		info->UE_state = CONNECTED;
	}
}

void send_file(struct conn_pair connection, int *flag)
{
	const char * filename = "BEAUTFUL_BADger.jpg";
	FILE * my_beautiful_badger = fopen(filename, "rb");

	fseek(my_beautiful_badger, 0, SEEK_END);
	size_d filesize = ftell(my_beautiful_badger);
	fseek(my_beautiful_badger, 0, SEEK_SET);

	struct FILE_DATA file_msg;

	memcpy(file_msg.file_name, filename, 20);

	file_msg.size = filesize;
	memset(file_msg.data, 0, 500000);
	fread(file_msg.data, sizeof(char), filesize, my_beautiful_badger);

	send_msg(connection, &file_msg, sizeof(struct FILE_DATA));

	*flag = 1;
}

int receive_msg(int fd, void *buffer, size_t buffer_size)
{
	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, buffer, buffer_size, 0,
				(struct sockaddr *)&clientConfig, &ca_len);
	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct DCI_MESSAGE))
		{
			return -1;
		}
		else
		{
			return 0;			
		}
	}
	return -1;
}

int send_msg(struct conn_pair connection, void *buffer, size_t buffer_size)
{
	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	other.sin_addr.s_addr = INADDR_ANY;
	other.sin_port = htons(connection.port);

    if(sendto(connection.sock, buffer, buffer_size, 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("UCI send error: ");
		return -1;
    }
	return 0;
}

void drop_packets(struct eNB_conn_info connections)
{
	int drop[1024];

	receive_msg(connections.broadcast.sock, &drop, 1024);
	receive_msg(connections.dl_sch.sock, &drop, 1024);
	receive_msg(connections.pdcch.sock, &drop, 1024);
}

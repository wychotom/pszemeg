#include "header.h"

void receive_broadcast(int fd, struct UE_INFO *info, struct MIB_MESSAGE *mib_ret)
{
	struct MIB_MESSAGE mib_msg;
	int retval = receive_msg(fd, &mib_msg, sizeof(struct MIB_MESSAGE));
	
	if(retval == 0)
	{
		if(info->UE_state == 0)
			info->UE_state = 1;

		mib_ret->broadcast_port = mib_msg.broadcast_port;
		mib_ret->prach_port = mib_msg.prach_port;
		mib_ret->dl_sch_port = mib_msg.dl_sch_port;
		mib_ret->ul_sch_port = mib_msg.ul_sch_port;
		mib_ret->pdcch_port = mib_msg.pdcch_port;
		mib_ret->pucch_port = mib_msg.pucch_port;
		#ifdef DEBUG
		printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
				mib_msg.broadcast_port, mib_msg.prach_port, mib_msg.dl_sch_port,
				mib_msg.ul_sch_port, mib_msg.pdcch_port, mib_msg.pucch_port);
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
		printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
				"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
				dci_msg.format0_a_flag, dci_msg.freqency_hooping, dci_msg.riv, dci_msg.mcs,
				dci_msg.ndi, dci_msg.tpc, dci_msg.cyclic_shift, dci_msg.cqi_request);
        #endif

		info->uplink_power_control.drx_cycle_type = dci_msg.drx_config.drx_cycle_type;
	}
}

void send_random_access_preamble(struct conn_pair connection, struct UE_INFO * info)
{
    struct RANDOM_ACCESS_PREAMBLE rap_msg;
    const short int preamble_identifier = 1337;

    rap_msg.preamble = preamble_identifier;
    rap_msg.RA_RNTI = info->RNTI;
    rap_msg.checksum = preamble_identifier + info->RNTI;

	int retval = send_msg(connection, &rap_msg, sizeof(struct RANDOM_ACCESS_PREAMBLE));

	if(retval == 0)
	{
	    info->UE_state = 1; // sending rap all the time
	}
}

void receive_random_access_response(int fd, struct UE_INFO *info)
{
	struct RANDOM_ACCESS_RESPONSE rar_msg;

	int retval = receive_msg(fd, &rar_msg, sizeof(struct RANDOM_ACCESS_RESPONSE));
	if(retval == 0)
	{
		// int calc_check_sum = 0;

		// calc_check_sum += rar_msg.timing_advance;
		// calc_check_sum += rar_msg.uplink_resource_grant;
		// calc_check_sum += rar_msg.temporary_c_rnti;
		if(rar_msg.RA_RNTI == info->RNTI)
			info->RNTI = rar_msg.temporary_c_rnti;

		#ifdef DEBUG
		printf("\ntim_adv = %d urg = %d, temp_rnti = %d, checksum = %ld\n",
			rar_msg.timing_advance, rar_msg.uplink_resource_grant,
			rar_msg.temporary_c_rnti, rar_msg.checksum);
		#endif
		info->UE_state = 2;
	}
}

void send_uci(struct conn_pair connection, struct UE_INFO *info)
{
	struct UPLINK_CONTROL_INFORMATION uci_msg;

	uci_msg.ue_info = *info;
	uci_msg.C_RNTI = info->RNTI;
	uci_msg.harq_ack = 1;
	uci_msg.cqi = 1;
	uci_msg.scheduling_request = 1;
	
	int retval = send_msg(connection, &uci_msg, sizeof(struct UPLINK_CONTROL_INFORMATION));
	if(retval == 0)
	{
		;//
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
		info->UE_state = 3;
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
			info->uplink_power_control.drx_cycle_type = rrc_msg.uplink_power_control.drx_cycle_type;
			info->uplink_power_control.short_drx_timer = rrc_msg.uplink_power_control.short_drx_timer;
			info->uplink_power_control.long_drx_timer = rrc_msg.uplink_power_control.long_drx_timer;
			info->uplink_power_control.on_duration_timer = rrc_msg.uplink_power_control.on_duration_timer;


			info->ul_sch_config = rrc_msg.ul_sch_config;
			info->UE_state = 4;
			
			#ifdef DEBUG
			printf("C_RNTI = %d\nSRB_ID = %d\nDL_AM_RLC = %d\nUL_AM_RLC = %d"
					"\nUL_SCH_CONF = %d\nPHR_CONF = %d\nON_DUR_TIMER = %d\n"
					,rrc_msg.C_RNTI, rrc_msg.srb_identity, rrc_msg.dl_am_rlc, rrc_msg.ul_am_rlc,
					rrc_msg.ul_sch_config, rrc_msg.phr_config,
					rrc_msg.uplink_power_control.on_duration_timer);
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
		info->UE_state = 6;
	}
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
	int broadcast = 1;

	other.sin_family = AF_INET;
	//other.sin_addr.s_addr = inet_addr("192.168.40.255");
	other.sin_addr.s_addr = INADDR_ANY;
	other.sin_port = htons(connection.port);

	//setsockopt(connection.sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    if(sendto(connection.sock, buffer, buffer_size, 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("UCI send error: ");
		return -1;
    }
	return 0;
}

void drop_packets(struct eNB_conn_info connections)
{
	int drop[1024] = {};

	receive_msg(connections.broadcast.sock, &drop, 1024);
	receive_msg(connections.dl_sch.sock, &drop, 1024);
	receive_msg(connections.pdcch.sock, &drop, 1024);
}
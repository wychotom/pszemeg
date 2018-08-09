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
		printf("TAMADA\n");
	}
}

void send_rrc_setup_complete(struct conn_pair connection, struct UE_INFO *info)
{

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
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(connection.port);
	
    if(sendto(connection.sock, buffer, buffer_size, 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("UCI send error: ");
		return -1;
    }
	return 0;
}
#include "header.h"

void receive_broadcast(int fd, struct UE_INFO *info, struct MIB_MESSAGE *mib_ret)
{
	struct MIB_MESSAGE mib_msg;
	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &mib_msg, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);
	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct MIB_MESSAGE))
		{
			return;
		}
		else
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
}

void receive_dci(int fd, struct UE_INFO *info)
{
	struct DCI_MESSAGE dci_msg;

	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &dci_msg, sizeof(struct DCI_MESSAGE), 0,
				(struct sockaddr *)&clientConfig, &ca_len);

	int calc_check_sum = 0;

	calc_check_sum += dci_msg.format0_a_flag;
	calc_check_sum += dci_msg.freqency_hooping;
	calc_check_sum += dci_msg.riv;
	calc_check_sum += dci_msg.mcs;
	calc_check_sum += dci_msg.ndi;
	calc_check_sum += dci_msg.tpc;
	calc_check_sum += dci_msg.cyclic_shift;
	calc_check_sum += dci_msg.cqi_request;//tbi

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct DCI_MESSAGE))
		{
			return;
		}
		else
		{
			#ifdef DEBUG
			printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
					"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
					dci_msg.format0_a_flag, dci_msg.freqency_hooping, dci_msg.riv, dci_msg.mcs,
					dci_msg.ndi, dci_msg.tpc, dci_msg.cyclic_shift, dci_msg.cqi_request);
			#endif
			
		}
	}
}

void send_random_access_preamble(struct conn_pair connection, struct UE_INFO * info)
{
    struct RANDOM_ACCESS_PREAMBLE rap_msg;
    const short int preamble_identifier = 1337;

	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(connection.port);


    rap_msg.preamble = preamble_identifier;
    rap_msg.RA_RNTI = info->RNTI;
    rap_msg.checksum = preamble_identifier + info->RNTI;

    if(sendto(connection.sock, &rap_msg, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("Random access preamble error: ");
    }
	else
	{
	    info->UE_state = 1; // sending rap all the time
	}
}

void receive_random_access_response(int fd, struct UE_INFO *info)
{
	struct RANDOM_ACCESS_RESPONSE rar_msg;

	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &rar_msg, sizeof(struct RANDOM_ACCESS_RESPONSE), 0,
				(struct sockaddr *)&clientConfig, &ca_len);

	int calc_check_sum = 0;

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct DCI_MESSAGE))
		{
			return;
		}
		else
		{
			calc_check_sum += rar_msg.timing_advance;
			calc_check_sum += rar_msg.uplink_resource_grant;
			calc_check_sum += rar_msg.temporary_c_rnti;

			info->RNTI = rar_msg.temporary_c_rnti;

			//if(calc_check_sum == rar_msg.checksum)
			//{
				#ifdef DEBUG
				printf("\ntim_adv = %d urg = %d, temp_rnti = %d, checksum = %ld\n",
					rar_msg.timing_advance, rar_msg.uplink_resource_grant,
					rar_msg.temporary_c_rnti, rar_msg.checksum);
				#endif
				

				info->UE_state = 2;
			//}

			
		}
	}
}

void send_uci(struct conn_pair connection, struct UE_INFO *info)
{
	struct UPLINK_CONTROL_INFORMATION uci_msg;

	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(connection.port);

	uci_msg.ue_info = *info;
	uci_msg.C_RNTI = info->RNTI;
	uci_msg.harq_ack = 1;
	uci_msg.cqi = 1;
	uci_msg.scheduling_request = 1;
	
    if(sendto(connection.sock, &uci_msg, sizeof(struct UPLINK_CONTROL_INFORMATION), 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("UCI send error: ");
    }

}

void send_rrc_req(struct conn_pair connection, struct UE_INFO *info)
{
	struct RRC_CONN_REQUEST rrc_msg;

	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(connection.port);

	enum establishment_causes cause = mobile_originating_signaling;

	rrc_msg.C_RNTI = info->RNTI;
	rrc_msg.UE_identity = 3;//should be random from 0 to 2^40
	rrc_msg.establishment_cause = cause;

    if(sendto(connection.sock, &rrc_msg, sizeof(struct RRC_CONN_REQUEST),
			0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("RRC_req error: ");
    }
	else
	{
		info->UE_state = 3;
	}
}

void receive_rrc_setup(int fd, struct UE_INFO *info)
{
	struct RRC_CONN_SETUP rrc_msg;

	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &rrc_msg, sizeof(struct RANDOM_ACCESS_RESPONSE), 0,
				(struct sockaddr *)&clientConfig, &ca_len);

	int calc_check_sum = 0;

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct DCI_MESSAGE))
		{
			return;
		}
		else
		{
			//if(calc_check_sum == rar_msg.checksum)
			//{
				#ifdef DEBUG
				printf("\ntim_adv = %d urg = %d, temp_rnti = %d, checksum = %ld\n",
					rar_msg.timing_advance, rar_msg.uplink_resource_grant,
					rar_msg.temporary_c_rnti, rar_msg.checksum);
				#endif
				

				info->UE_state = 4;
			//}

			
		}
	}
}
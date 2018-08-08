#include "header.h"

void receive_init_broadcast_msg(int *flag, struct MIB_MESSAGE *return_MIB)
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
	broadcast = 1;

	retval = setsockopt(broadcast_socket, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));
	if(retval == -1)
	{
		perror("Failed on so_reuseaddr\n");
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

	ca_len = sizeof(clientConfig);
    recvbytes = recvfrom(broadcast_socket, return_MIB, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct MIB_MESSAGE))
		{
			return;
		}
		else
		{
			*flag = broadcast_socket;
			return;
		}
	}
	if(recvbytes == 0)
    {
        perror("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
    return;
}

void receive_broadcast(int fd, struct UE_INFO *info)
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
			printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
					mib_msg.broadcast_port, mib_msg.prach_port, mib_msg.dl_sch_port,
					mib_msg.ul_sch_port, mib_msg.pdcch_port, mib_msg.pucch_port);
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
			printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
					"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
					dci_msg.format0_a_flag, dci_msg.freqency_hooping, dci_msg.riv, dci_msg.mcs,
					dci_msg.ndi, dci_msg.tpc, dci_msg.cyclic_shift, dci_msg.cqi_request);
		}
	}
}

void send_random_access_preamble(int fd, int port, struct UE_INFO * info)
{
    struct RANDOM_ACCESS_PREAMBLE rap_msg;
    const short int preamble_identifier = 1337;

	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	//other.sin_addr.s_addr = inet_addr("192.168.40.255");
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(port);

    info->UE_state = 1; // sending rap all the time

    rap_msg.preamble = preamble_identifier;
    rap_msg.RA_RNTI = info->RNTI;
    rap_msg.checksum = preamble_identifier + info->RNTI;

    if(sendto(fd, &rap_msg, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("Random access preamble error: ");
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

			//if(calc_check_sum == rar_msg.checksum)
			//{
				printf("\ntim_adv = %d urg = %d, temp_rnti = %d, checksum = %ld\n",
					rar_msg.timing_advance, rar_msg.uplink_resource_grant,
					rar_msg.temporary_c_rnti, rar_msg.checksum);

				info->UE_state = 2;
			//}

			
		}
	}
}

void send_uci(int fd, int port, struct UE_INFO *info)
{
	struct UCI_MESSAGE uci_msg;

	struct sockaddr_in other;
	unsigned int otherlen = sizeof(other);

	other.sin_family = AF_INET;
	other.sin_addr.s_addr = htonl(INADDR_ANY);
	other.sin_port = htons(port);

	//info->UE_state = 1; // sending rap all the time

	uci_msg.info = *info;
	uci_msg.RA_RNTI = info->RNTI;
	uci_msg.harq_ack = 1;
	uci_msg.cqi = 1;
	uci_msg.scheduling_request = 1;

    if(sendto(fd, &uci_msg, sizeof(struct UCI_MESSAGE), 0, (struct sockaddr *)&other, otherlen) == -1)
    {
        perror("UCI send error: ");
    }

}
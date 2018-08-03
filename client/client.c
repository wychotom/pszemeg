#include "header.h"

int main(int argc, char ** argv)
{
	printf("Uniq name = %d\n", get_unique_name());

	int waiting_for_broadcast = 0;
	struct MIB_MESSAGE ports;//broadcast only
	struct eNB_conn_info connection_information;//Ideally it would be an array, information about multiple eNB

	while(!waiting_for_broadcast)
	{
		receive_broadcast_msg(&waiting_for_broadcast, &ports);
		if(waiting_for_broadcast)
		{
			// connection_information.broadcast.port = ports.broadcast_port;
			// connection_information.broadcast.sock = 0;

			// connection_information.prach.port = ports.prach_port;
			// connection_information.prach.sock = 0;

			// connection_information.dl_sch.port = ports.dl_sch_port;
			// connection_information.dl_sch.sock = 0;

			// connection_information.ul_sch.port = ports.ul_sch_port;
			// connection_information.ul_sch.port = 0;

			// connection_information.pdcch.port = ports.pdcch_port;
			// connection_information.pdcch.sock = 0;

			// connection_information.pucch.port = ports.pucch_port;
			// connection_information.pucch.sock = 0;
			handletraffic(ports);
		}
	}



	printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
			ports.broadcast_port, ports.prach_port, ports.dl_sch_port,
			ports.ul_sch_port, ports.pdcch_port, ports.pucch_port);



	/*
	int mainSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == -1)
	{
		printf("error while creating socket.\n");
		return 1;
	}
	char buf[BUFFER_LENGTH];

	
	struct sockaddr_in sa;

	int sh = socket(AF_INET, SOCK_STREAM, 0);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(port);
	if(connect(sh, (struct sockaddr *)&sa, sizeof(sa))!= -1)
	{
		//SHIT THATS NOT GUD
		send_message("xDDD", sh);
		sleep(2);
		char buf[BUFFER_LENGTH];
		while(1)
		{
			memset(buf, '\0', BUFFER_LENGTH);
			fgets(buf, BUFFER_LENGTH, stdin);
			buf[strcspn(buf, "\n")] = 0;
			send_message(buf, sh);
			sleep(1);
		}
		
	}
	else
	{
		printf("Server is offline.\n");
	}*/
	return 0;
}

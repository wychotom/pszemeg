#include "header.h"

int main(int argc, char ** argv)
{
	printf("Uniq name = %s\n", getUniqueName());

	int waiting_for_broadcast = 0;

	while(!waiting_for_broadcast)
	{
		receive_broadcast_msg(&waiting_for_broadcast);
	}


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

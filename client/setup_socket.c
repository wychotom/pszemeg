#include "header.h"

#include <sys/fcntl.h>
#include <errno.h>

void setup_broadcast_socket(int * fd)
{
	*fd = socket(AF_INET, SOCK_DGRAM, 0);

	if(*fd == -1)
	{
		perror("Can't create a socket: ");
		exit(EXIT_FAILURE);
	}

	int broadcast = 1;
	int retval = setsockopt(*fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    if(retval == -1)
	{
		perror("Failed on setsockopt\n");
	}
	broadcast = 1;

	retval = setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));
	if(retval == -1)
	{
		perror("Failed on so_reuseaddr\n");
	}


	struct sockaddr_in socketConfig;
    //memset(&socketConfig, 0, sizeof(socketConfig));

    socketConfig.sin_family = AF_INET;
	socketConfig.sin_port = htons(BROADCAST_PORT);
	socketConfig.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(*fd, (struct sockaddr *)&socketConfig, sizeof(struct sockaddr));
    if(retval == -1)
	{
		perror("Failed on bind\n");
	}
}

void setup_socket(int * sockfd, int port)
{
	int errflag = 1;

	struct sockaddr_in sa;

	*sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &errflag, sizeof(errflag));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);

    errflag = bind(*sockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr));

	if (errflag == -1)
	{
		if(errno == EINPROGRESS)
		{
			perror("bind in progress: ");
		}
		else
			perror("bind err: ");
	}
}

int set_non_block(int sockfd)
{
	int flags, retval;

	flags = fcntl (sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	retval = fcntl (sockfd, F_SETFL, flags);
	if (retval == -1)
	{
		perror ("fcntl");
		return -1;
	}
	return 0;
}

void add_socket_epoll(struct epoll_event *ev, int *efd, int *to_watch)
{
	ev->events = EPOLLIN;
	ev->data.fd = *to_watch;
	if (epoll_ctl(*efd, EPOLL_CTL_ADD, *to_watch, ev) == -1) {
		perror("epoll_ctl: xd");
		exit(EXIT_FAILURE);
	}
}
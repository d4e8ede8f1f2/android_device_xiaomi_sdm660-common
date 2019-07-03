
#include "IPACM_Log.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <asm/types.h>
#include <linux/if.h>
#include <sys/un.h>
#include <errno.h>
#include <IPACM_Defs.h>

/* start IPACMDIAG socket*/
int create_socket(int *sockfd)
{

  if ((*sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == IPACM_FAILURE)
  {
    perror("Error creating ipacm_log socket\n");
    return IPACM_FAILURE;
  }

  if(fcntl(*sockfd, F_SETFD, FD_CLOEXEC) < 0)
  {
    perror("Couldn't set ipacm_log Close on Exec\n");
  }

  return IPACM_SUCCESS;
}

void ipacm_log_send( void * user_data)
{
	ipacm_log_buffer_t ipacm_log_buffer;
	int numBytes=0, len;
	struct sockaddr_un ipacmlog_socket;
	static int ipacm_log_sockfd = 0;

	if(ipacm_log_sockfd == 0)
	{
		/* start ipacm_log socket */
		if(create_socket(&ipacm_log_sockfd) < 0)
		{
			printf("unable to create ipacm_log socket\n");
			return;
		}
		printf("create ipacm_log socket successfully\n");
	}
	ipacmlog_socket.sun_family = AF_UNIX;
	strlcpy(ipacmlog_socket.sun_path, IPACMLOG_FILE,sizeof(ipacmlog_socket.sun_path));
	len = strlen(ipacmlog_socket.sun_path) + sizeof(ipacmlog_socket.sun_family);

	memcpy(ipacm_log_buffer.user_data, user_data, MAX_BUF_LEN);

	//printf("send : %s\n", ipacm_log_buffer.user_data);
	if ((numBytes = sendto(ipacm_log_sockfd, (void *)&ipacm_log_buffer, sizeof(ipacm_log_buffer.user_data), 0,
			(struct sockaddr *)&ipacmlog_socket, len)) == -1)
	{
		printf("Send Failed(%d) %s \n",errno,strerror(errno));
		return;
	}
	return;
}

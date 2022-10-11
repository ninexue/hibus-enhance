#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <hibus.h>

#include <hibus-busybox.h>

#define SOCKET_PATH			"/var/tmp/hibus.sock"
#define APP_NAME_SUMMER		"cn.summer.hzy"
#define RUNNER_NAME_TEST	"test"

int main(void)
{
	int ret_code = 0;
	int fd_hibus = -1;
	hibus_conn * hibus_context = NULL;

	fd_hibus = hibus_connect_via_unix_socket(SOCKET_PATH, APP_NAME_SUMMER, RUNNER_NAME_TEST, &hibus_context);
	if((fd_hibus <= 0) || (hibus_context == NULL))
	{
		fprintf(stderr, "Connects to HIBUS server error, %s.\n", hibus_get_err_message(fd_hibus));
		exit(1);
	}
//	hibus_conn_set_user_data(hibus_context, &device);

	while(1)
    {
        ret_code = hibus_wait_and_dispatch_packet(hibus_context, 1000);
        if(ret_code)
            fprintf(stderr, "For hibus_wait_and_dispatch_packet, %s.\n", hibus_get_err_message(ret_code));
    }

	hibus_disconnect(hibus_context);
	return 0;
}

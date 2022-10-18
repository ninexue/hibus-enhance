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

#include "hibus_busybox.h"

#define SOCKET_PATH			"/var/tmp/hibus.sock"
#define APP_NAME_SUMMER		"cn.summer.hzy"
#define RUNNER_NAME_TEST	"test"
#define RUNNER_NAME_BUSYBOX	"busybox"

static int
chdir_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("chdir ================================================= %s\n", ret_value);
    return 0;
}

static int
ls_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("ls ================================================= %s\n", ret_value);
    return 0;
}

static int
touch_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("touch ================================================= %s\n", ret_value);
    return 0;
}

static int
unlink_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("unlink ================================================= %s\n", ret_value);
    return 0;
}

static int
mkdir_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("mkdir ================================================= %s\n", ret_value);
    return 0;
}

static int
rm_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("rm ================================================= %s\n", ret_value);
    return 0;
}

static int
rmdir_handler(hibus_conn* conn, const char* from_endpoint,
			const char* method_name, const char *call_id,
			int ret_code, const char* ret_value)
{
    printf("rmdir ================================================= %s\n", ret_value);
    return 0;
}

int main(void)
{
	int ret_code = 0;
	int fd_hibus = -1;
	hibus_conn * hibus_context = NULL;
    char *endpoint = NULL;
	const char *call_id = NULL;
	uid_t euid = geteuid();
	char command[100];

	fd_hibus = hibus_connect_via_unix_socket(SOCKET_PATH, APP_NAME_SUMMER,
											RUNNER_NAME_TEST, &hibus_context);
	if((fd_hibus <= 0) || (hibus_context == NULL))
	{
		fprintf(stderr, "Connects to HIBUS server error, %s.\n",
											hibus_get_err_message(fd_hibus));
		exit(1);
	}

    endpoint = hibus_assemble_endpoint_name_alloc(HIBUS_LOCALHOST,
					APP_NAME_SUMMER, RUNNER_NAME_BUSYBOX);

	// chdir
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_CHDIR, command, 1000,
					chdir_handler, &call_id);
	// ls
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_LS, command, 1000,
					ls_handler, &call_id);

	// touch
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
					touch_handler, &call_id);

	// unlink
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_UNLINK, command, 1000,
					unlink_handler, &call_id);

	// mkdir
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_MKDIR, command, 1000,
					mkdir_handler, &call_id);

	// rm
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_RM, command, 1000,
					rm_handler, &call_id);

	// rmdir
	sprintf(command, "{\"euid\":%u, \"path\":\"/home/hzy\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
					METHOD_HIBUS_BUSYBOX_RMDIR, command, 1000,
					rmdir_handler, &call_id);

	while(1)
    {
        ret_code = hibus_wait_and_dispatch_packet(hibus_context, 1000);
        if(ret_code)
            fprintf(stderr, "For hibus_wait_and_dispatch_packet, %s.\n",
										hibus_get_err_message(ret_code));
    }

    free(endpoint);
	hibus_disconnect(hibus_context);

	return 0;
}

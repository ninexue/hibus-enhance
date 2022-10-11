#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <net/if.h>
#include <sys/timerfd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <arpa/inet.h>
#include <errno.h>

#include <hibus.h>
#include "helper.h"
#include "hibus-busybox.h"

hibus_user * init_runner(void *data)
{
	int fd_hibus_busybox = -1;
	hibus_user *user = NULL;

	user = calloc(sizeof(hibus_user), 1);
	if(user == NULL)
		return NULL;

	fd_hibus_busybox = hibus_connect_via_unix_socket(SOCKET_PATH,
				APP_NAME_HIBUS, RUNNER_NAME_BUSYBOX, &user->context);
    if((fd_hibus_busybox <= 0) || (user->context == NULL))
    {
    	fprintf(stderr, "BUSYBOX RUNNER: BUSYBOX connects to HIBUS server error, %s.\n", hibus_get_err_message(fd_hibus_busybox));
        return NULL;
    }

	user->fd = fd_hibus_busybox;
	user->data = data;
    hibus_conn_set_user_data(user->context, user);

	return user;
}

int deinit_runner(hibus_user *user)
{
/*
	int ret_code = 0;

    ret_code = hibus_register_procedure(hibus_context_inetd, METHOD_NET_OPEN_DEVICE, NULL, NULL, openDevice);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_NET_OPEN_DEVICE, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(hibus_context_inetd, METHOD_NET_CLOSE_DEVICE, NULL, NULL, closeDevice);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_NET_CLOSE_DEVICE, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(hibus_context_inetd, METHOD_NET_GET_DEVICES_STATUS, NULL, NULL, getNetworkDevicesStatus);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_NET_GET_DEVICES_STATUS, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_event(hibus_context_inetd, NETWORKDEVICECHANGED, NULL, NULL);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register event %s, %s.\n", NETWORKDEVICECHANGED, hibus_get_err_message(ret_code));
        return;
    }
*/
	return 0;
}

int user_login(void)
{
	printf("it is user_login\n");
	return 0;
}

int user_logout(void)
{
	printf("it is user_logout\n");
	return 0;
}

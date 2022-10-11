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
#include "hibus-busybox.h"
#include "filesystem.h"
#include "interface.h"

hibus_user * init_runner(void *data)
{
	int fd_hibus_busybox = -1;
	hibus_user *user = NULL;

	user = calloc(sizeof(hibus_user), 1);
	if(user == NULL)
		return NULL;

	// connect to hibus
	fd_hibus_busybox = hibus_connect_via_unix_socket(SOCKET_PATH,
				APP_NAME_HIBUS, RUNNER_NAME_BUSYBOX, &user->context);
    if((fd_hibus_busybox <= 0) || (user->context == NULL))
    {
    	fprintf(stderr, "BUSYBOX RUNNER: BUSYBOX connects to HIBUS server error, %s.\n", hibus_get_err_message(fd_hibus_busybox));
        return NULL;
    }

	user->fd = fd_hibus_busybox;
	user->data = data;

	// add user data
    hibus_conn_set_user_data(user->context, user);

	// register remote procedure and event
	fs_register(user->context);

	return user;
}

int deinit_runner(hibus_user *user)
{
	int ret = 0;

	if(user == NULL)
		return -1;

	if(user->context)
	{
		// revoke remote procedure and event here
		fs_revoke(user->context);

		// disconnect to hibus server
		hibus_disconnect(user->context);

		// free user data
		free(user);
	}
	else
		ret = -1;

	return ret;
}

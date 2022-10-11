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
#include <hibox/json.h>

#include "hibus-busybox.h"
#include "interface.h"

#ifndef BUILD_LIBRARY

int main(void)
{
	int ret_code = 0;
	hibus_user *user = NULL;

	if(0)
	{
	hibus_json *jo = NULL;
	hibus_json *jo_tmp = NULL;

	json_object_object_get_ex(jo, "device", &jo_tmp);
	}

	user = init_runner(NULL);

	if(user == NULL)
		exit(1);

    while(1)
    {
        ret_code = hibus_wait_and_dispatch_packet(user->context, 1000);
        if(ret_code)
            fprintf(stderr, "Busybox runner: error for hibus_wait_and_dispatch_packet, %s.\n", hibus_get_err_message(ret_code));
    }

	deinit_runner(user);

	return 0;
}

#endif

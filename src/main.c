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
#include <dlfcn.h>
#include <libgen.h>

#include <hibus.h>
#include <hibox/json.h>
#include "hibus_busybox.h"

int main(void)
{
	char lib_path[256];
	void *handle = NULL;
	hibus_conn *conn = NULL;
	bool (*init_lib)(hibus_conn **, void *);
	int (*deinit_lib)(void);
	int ret = 0;

	// get the library path
	readlink("/proc/self/exe", lib_path, 256);
    dirname(lib_path);
    dirname(lib_path);
	strcat(lib_path, "/lib/libhibus-busybox.so");

	// load library
    handle = dlopen(lib_path, RTLD_LAZY);
    if(!handle) {
       	fprintf(stderr, "Error: Load dynamic library error.\n");;
        return ERROR_LOAD_LIB;
    }

	// get init function
	init_lib = (bool (*)(hibus_conn **, void *))dlsym(handle, "init_runner");

    if(dlerror() != NULL)
	{
       	fprintf(stderr, "Error: Find init_runner in lib error.\n");;
        dlclose(handle);
        return ERROR_FIND_SYM;
    }

	// execute init function
	ret = init_lib(&conn, NULL);
	if(ret)
	{
       	fprintf(stderr, "Error: Execute init_runner in lib error.\n");;
        dlclose(handle);
        return ERROR_EXEC_LIB_FUNC;
	}

	// loop for execution
    while(1)
    {
        ret = hibus_wait_and_dispatch_packet(conn, 1000);
        if(ret)
            fprintf(stderr, "Busybox runner: error for "
							"hibus_wait_and_dispatch_packet, %s.\n",
							hibus_get_err_message(ret));
    }

	// get deinit function
	deinit_lib = (int (*)(void))dlsym(handle, "deinit_runner");

    if(dlerror() != NULL)
	{
       	fprintf(stderr, "Error: Find deinit_runner in lib error.\n");;
        dlclose(handle);
        return ERROR_FIND_SYM;
    }

	// execute deinit function
	ret = deinit_lib();
	if(ret)
	{
       	fprintf(stderr, "Error: Execute deinit_runner in lib error.\n");;
        dlclose(handle);
        return ERROR_EXEC_LIB_FUNC;
	}

	// close library
	dlclose(handle);

	return 0;
}

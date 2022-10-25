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

#include "hibus_busybox.h"
#include "helper.h"
#include "filesystem.h"
#include "interface.h"

static hibus_user *hiuser = NULL;

int init_runner(hibus_conn **conn, void *data)
{
    int fd_hibus_busybox = -1;

    if(conn == NULL)
        return ERROR_BUSYBOX_INVALID_PARAM;

    hiuser = calloc(sizeof(hibus_user), 1);
    if(hiuser == NULL)
        return ERROR_BUSYBOX_INSUFFICIENT_MEM;

    // connect to hibus
    fd_hibus_busybox = hibus_connect_via_unix_socket(SOCKET_PATH,
                APP_NAME, RUNNER_NAME_BUSYBOX, &hiuser->context);
    if((fd_hibus_busybox <= 0) || (hiuser->context == NULL))
    {
        fprintf(stderr, "BUSYBOX RUNNER: BUSYBOX connects to HIBUS server"
                        "error, %s.\n", hibus_get_err_message(fd_hibus_busybox));
        return ERROR_BUSYBOX_CONNECT_HIBUS;
    }

    hiuser->fd = fd_hibus_busybox;
    hiuser->data = data;
    hiuser->cwd[0] = 0;

    hibus_conn_set_user_data(hiuser->context, hiuser);

    // register remote procedure and event
    fs_register(hiuser->context);

    *conn = hiuser->context;

    return ERROR_BUSYBOX_OK;
}

int deinit_runner(void)
{
    int ret = ERROR_BUSYBOX_OK;

    if(hiuser == NULL)
        return ERROR_BUSYBOX_INVALID_PARAM;

    if(hiuser->context)
    {
        // revoke remote procedure and event here
        fs_revoke(hiuser->context);

        // disconnect to hibus server
        hibus_disconnect(hiuser->context);

        // free hiuser
        free(hiuser);
        hiuser = NULL;
    }
    else
        ret = ERROR_BUSYBOX_INVALID_PARAM;

    return ret;
}

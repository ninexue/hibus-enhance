#ifndef __HIBUS_BUSYBOX_INTERFACE__
#define __HIBUS_BUSYBOX_INTERFACE__

#define SOCKET_PATH				"/var/tmp/hibus.sock"

#ifdef BUILD_HIBUS_NATIVE
	#define APP_NAME       		"cn.fmsoft.hybridos.hibus"
#else
	#define APP_NAME			"cn.summer.hzy"
#endif

#define RUNNER_NAME_BUSYBOX    	"busybox"

struct busybox_procedure
{
    const char *name;
    hibus_method_handler handler;
};

struct busybox_event
{
    const char *name;
};

// structure for hibus user
struct _hibus_user
{
    int fd;
	hibus_conn *context;
    void *data;
};

typedef struct _hibus_user	hibus_user;

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
 * Subroutine:  init_runner 
 * Function:    connect to hibus and register procedure and event
 * Input:       conn[I/O], get hibus_conn for connection
 *              data[I], user data
 * Output:      error code of operation, 0 for successful.
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
int init_runner(hibus_conn **conn, void *data);


/****************************************************************************
 * Subroutine:  deinit_runner 
 * Function:    disconnect to hibus and revoke procedure and event
 * Input:       none
 * Output:      error code of operation, 0 for successful.
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
int deinit_runner(void);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_INTERFACE__

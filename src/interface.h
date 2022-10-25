#ifndef __HIBUS_BUSYBOX_INTERFACE__
#define __HIBUS_BUSYBOX_INTERFACE__

#define LIB_VISIBLE __attribute__((visibility("default")))

#define SOCKET_PATH         "/var/tmp/hibus.sock"

#ifdef BUILD_HIBUS_NATIVE
    #define APP_NAME        "cn.fmsoft.hybridos.hibus"
#else
    #define APP_NAME        "cn.summer.hzy"
#endif

#define RUNNER_NAME_BUSYBOX "busybox"

#define	PROCEDURE_PARAM	hibus_conn* conn, const char* from_endpoint, \
            const char* to_method, const char* method_param, int *err_code

// for register procedure
struct busybox_procedure
{
    const char *name;
    hibus_method_handler handler;
};

// for register event
struct busybox_event
{
    const char *name;
};

// for store user define environment parameter
struct env_param
{
    char *key;
    char *value;
    struct env_param *next;
};

// structure for hibus user
struct _hibus_user
{
    int fd;
    hibus_conn *context;
    char cwd[PATH_MAX];
    struct env_param *env;
    void *data;
};

typedef struct _hibus_user  hibus_user;

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
LIB_VISIBLE int init_runner(hibus_conn **conn, void *data);


/****************************************************************************
 * Subroutine:  deinit_runner
 * Function:    disconnect to hibus and revoke procedure and event
 * Input:       none
 * Output:      error code of operation, 0 for successful.
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
LIB_VISIBLE int deinit_runner(void);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_INTERFACE__

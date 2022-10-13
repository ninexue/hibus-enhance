#ifndef __HIBUS_BUSYBOX_INTERFACE__
#define __HIBUS_BUSYBOX_INTERFACE__

#define SOCKET_PATH				"/var/tmp/hibus.sock"

#ifdef BUILD_HIBUS_NATIVE
	#define APP_NAME       		"cn.fmsoft.hybridos.hibus"
#else
	#define APP_NAME			"cn.summer.hzy"
#endif

#define RUNNER_NAME_BUSYBOX    	"busybox"


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

// initialize library, connect to hibus and register procedure and event
int init_runner(hibus_conn **con, void *data);

// deinitialize library, free the resource
int deinit_runner(void);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_INTERFACE__

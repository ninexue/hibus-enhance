#ifndef __HIBUS_BUSYBOX_INTERFACE__
#define __HIBUS_BUSYBOX_INTERFACE__

#define SOCKET_PATH				"/var/tmp/hibus.sock"
#ifdef BUILD_LIBRARY
	#define APP_NAME       		"cn.fmsoft.hybridos.hibus"
#else
	#define APP_NAME			"cn.summer.hzy"
#endif
#define RUNNER_NAME_BUSYBOX    	"busybox"


// structure for user
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

hibus_user * init_runner(void *data);
int deinit_runner(hibus_user *user);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_INTERFACE__

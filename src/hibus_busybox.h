#ifndef __HIBUS_BUSYBOX_H__
#define __HIBUS_BUSYBOX_H__

// define error
#define	ERROR_BUSYBOX_OK				0
#define	ERROR_BUSYBOX_INVALID_PARAM		-1
#define	ERROR_BUSYBOX_INSUFFICIENT_MEM	-2
#define	ERROR_BUSYBOX_CONNECT_HIBUS		-3
#define	ERROR_BUSYBOX_WRONG_PROCEDURE	-4
#define	ERROR_BUSYBOX_WRONG_JSON		-5

#define	ERROR_LOAD_LIB					-100
#define	ERROR_FIND_SYM					-101
#define	ERROR_EXEC_LIB_FUNC				-102

// for file system remote invocation
#define METHOD_HIBUS_BUSYBOX_LS		"ls"
#define METHOD_HIBUS_BUSYBOX_RM		"rm"
#define METHOD_HIBUS_BUSYBOX_RMDIR	"rmdir"
#define METHOD_HIBUS_BUSYBOX_MKDIR	"mkdir"
#define METHOD_HIBUS_BUSYBOX_UNLINK	"unlink"
#define METHOD_HIBUS_BUSYBOX_TOUCH	"touch"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_H__

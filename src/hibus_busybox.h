#ifndef __HIBUS_BUSYBOX_H__
#define __HIBUS_BUSYBOX_H__

// define error
#define ERROR_BUSYBOX_OK                0
#define ERROR_BUSYBOX_EACCES            -1
#define ERROR_BUSYBOX_EBUSY             -2
#define ERROR_BUSYBOX_EFAULT            -3
#define ERROR_BUSYBOX_EIO               -4
#define ERROR_BUSYBOX_EISDIR            -5
#define ERROR_BUSYBOX_ELOOP             -6
#define ERROR_BUSYBOX_ENAMETOOLONG      -7
#define ERROR_BUSYBOX_ENOENT            -8
#define ERROR_BUSYBOX_ENOMEM            -9
#define ERROR_BUSYBOX_ENOTDIR           -10
#define ERROR_BUSYBOX_EPERM             -11
#define ERROR_BUSYBOX_EROFS             -12
#define ERROR_BUSYBOX_EBADF             -13
#define ERROR_BUSYBOX_EINVAL            -14
#define ERROR_BUSYBOX_ESRCH             -15
#define ERROR_BUSYBOX_ENOSPC            -16
#define ERROR_BUSYBOX_ENOTEMPTY         -17
#define ERROR_BUSYBOX_EWOULDBLOCK       -18
#define ERROR_BUSYBOX_ETXTBSY           -19
#define ERROR_BUSYBOX_EOVERFLOW         -20
#define ERROR_BUSYBOX_EOPNOTSUPP        -21
#define ERROR_BUSYBOX_ENXIO             -22
#define ERROR_BUSYBOX_ENODEV            -23
#define ERROR_BUSYBOX_ENFILE            -24
#define ERROR_BUSYBOX_EDQUOT            -25
#define ERROR_BUSYBOX_EEXIST            -26
#define ERROR_BUSYBOX_EFBIG             -27
#define ERROR_BUSYBOX_EINTR             -28
#define ERROR_BUSYBOX_EMFILE            -29
#define ERROR_BUSYBOX_ENOSYS            -30
#define ERROR_BUSYBOX_INVALID_PARAM     -31
#define ERROR_BUSYBOX_INSUFFICIENT_MEM  -32
#define ERROR_BUSYBOX_CONNECT_HIBUS     -33
#define ERROR_BUSYBOX_WRONG_PROCEDURE   -34
#define ERROR_BUSYBOX_WRONG_JSON        -35
#define ERROR_BUSYBOX_WORKING_DIRECTORY -36
#define ERROR_BUSYBOX_UNKONOWN          -37

#define ERROR_LOAD_LIB                  -100
#define ERROR_FIND_SYM                  -101
#define ERROR_EXEC_LIB_FUNC             -102

// for file system remote invocation
#define METHOD_HIBUS_BUSYBOX_LS         "ls"
#define METHOD_HIBUS_BUSYBOX_RM         "rm"
#define METHOD_HIBUS_BUSYBOX_RMDIR      "rmdir"
#define METHOD_HIBUS_BUSYBOX_MKDIR      "mkdir"
#define METHOD_HIBUS_BUSYBOX_UNLINK     "unlink"
#define METHOD_HIBUS_BUSYBOX_TOUCH      "touch"
#define METHOD_HIBUS_BUSYBOX_CHDIR      "chdir"
#define METHOD_HIBUS_BUSYBOX_DF         "df"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_H__

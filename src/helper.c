#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <hibus.h>

#include "helper.h"

const char *op_errors[] = {
    "Success.",                                 	// OK
	"Permission denied.",							// EACCES
	"Device is busy.",								// EBUSY
	"Bad address.",									// EFAULT
	"Input/output error.",							// EIO
	"Is a directory.",								// EISDIR
	"Too many levels of symbolic links.",			// ELOOP
	"File name is too long.",						// ENAMETOOLONG
	"No such file or directory.",					// ENOENT
	"Insufficient kernel memory was available.",	// ENOMEM
	"Is not a directory.",							// ENOTDIR
	"Operation is not permitted.",					// EPERM
	"The file is on a read-only filesystem.",		// EROFS
	"Bad file descriptor.",							// EBADF
	"Invalid argument.",							// EINVAL
	"No such process.",								// ESRCH
	"No space left on device.",						// ENOSPC
	"Directory is not empty.", 						// ENOTEMPTY
	"Operation would block.", 						// EWOULDBLOCK
	"Text file is busy.", 							// ETXTBSY
	"Value is too large to be stored in data type.",// EOVERFLOW
	"Operation is not supported.", 					// EOPNOTSUPP
	"Device is not configured.", 					// ENXIO
	"Operation is not supported by device.", 		// ENODEV
	"Too many open files are in system.", 			// ENFILE
	"Disc quota is exceeded.", 						// EDQUOT
	"File exists.",	 								// EEXIST
	"File is too large.", 							// EFBIG
	"Interrupted system call.", 					// EINTR
	"Too many open files.",							// EMFILE
    "Invalid parameter.",             				// INVALID_PARAM
    "Insufficient memory.",    						// INSUFFICIENT_MEM
    "Can not connect to Hibus server.",         	// CONNECT_HIBUS
    "Wrong procedure name.",                    	// WRONG_PROCEDURE
    "Wrong Json format.",                       	// WRONG_JSON
	"Current working directory has not benn set.",	// WORKING_DIRECTORY
	"Unknow error.",								// UNKONOWN
};

uid_t get_usr_id(const char *name)
{
    struct passwd *p;

	if(name == NULL)
		return 0xFFFFFFFF;

    if((p = getpwnam(name)) == NULL)
		return 0xFFFFFFFF;

    return p->pw_uid;
}

void change_euid(const char* from_endpoint, uid_t euid)
{
	char host_name[HIBUS_LEN_HOST_NAME];

	hibus_extract_host_name(from_endpoint, host_name);

	if(strcmp(host_name, HIBUS_LOCALHOST) == 0)		// local
		seteuid(euid);
	else
	{
		uid_t nobody = get_usr_id("nobody");
		seteuid(nobody);
	}
}

void restore_euid(void)
{
	uid_t euid, ruid, suid;

	getresuid(&euid, &ruid, &suid);

	seteuid(suid);
}

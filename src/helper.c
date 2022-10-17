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
    "Invalid parameter.",             				// INVALID_PARAM
    "Insufficient memory.",    						// INSUFFICIENT_MEM
    "Can not connect to Hibus server.",         	// CONNECT_HIBUS
    "Wrong procedure name.",                    	// WRONG_PROCEDURE
    "Wrong Json format.",                       	// WRONG_JSON
	"File or directory does not exist.",			// FILE_EXIST
	"Can not open directory.",						// DIR_OPEN
	"Current working directory has not benn set.",	// WORKING_DICT
	"It is not a file.",							// NOT_FILE
	"It is not a directory.",						// NOT_DIRECTORY
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

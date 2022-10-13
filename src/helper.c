#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>

#include "helper.h"

uid_t get_usr_id(const char *name)
{
    struct passwd *p;

	if(name == NULL)
		return 0xFFFFFFFF;

    if((p = getpwnam(name)) == NULL)
		return 0xFFFFFFFF;

    return p->pw_uid;
}

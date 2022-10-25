#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <hibus.h>

#include "hibus_busybox.h"
#include "helper.h"
#include "interface.h"

const char *op_errors[] = {
    "Success.",                                     // OK
    "Permission denied.",                           // EACCES
    "Device is busy.",                              // EBUSY
    "Bad address.",                                 // EFAULT
    "Input/output error.",                          // EIO
    "Is a directory.",                              // EISDIR
    "Too many levels of symbolic links.",           // ELOOP
    "File name is too long.",                       // ENAMETOOLONG
    "No such file or directory.",                   // ENOENT
    "Insufficient kernel memory was available.",    // ENOMEM
    "Is not a directory.",                          // ENOTDIR
    "Operation is not permitted.",                  // EPERM
    "The file is on a read-only filesystem.",       // EROFS
    "Bad file descriptor.",                         // EBADF
    "Invalid argument.",                            // EINVAL
    "No such process.",                             // ESRCH
    "No space left on device.",                     // ENOSPC
    "Directory is not empty.",                      // ENOTEMPTY
    "Operation would block.",                       // EWOULDBLOCK
    "Text file is busy.",                           // ETXTBSY
    "Value is too large to be stored in data type.",// EOVERFLOW
    "Operation is not supported.",                  // EOPNOTSUPP
    "Device is not configured.",                    // ENXIO
    "Operation is not supported by device.",        // ENODEV
    "Too many open files are in system.",           // ENFILE
    "Disc quota is exceeded.",                      // EDQUOT
    "File exists.",                                 // EEXIST
    "File is too large.",                           // EFBIG
    "Interrupted system call.",                     // EINTR
    "Too many open files.",                         // EMFILE
    "Function is not supported.",                   // ENOSYS
    "Invalid parameter.",                           // INVALID_PARAM
    "Insufficient memory.",                         // INSUFFICIENT_MEM
    "Can not connect to Hibus server.",             // CONNECT_HIBUS
    "Wrong procedure name.",                        // WRONG_PROCEDURE
    "Wrong Json format.",                           // WRONG_JSON
    "Current working directory has not benn set.",  // WORKING_DIRECTORY
    "Unknow error.",                                // UNKONOWN
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

    if(strcmp(host_name, HIBUS_LOCALHOST) == 0)        // local
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

int get_error_code(void)
{
    int ret = ERROR_BUSYBOX_OK;
    switch(errno)
    {
        case EACCES:
            ret = ERROR_BUSYBOX_EACCES;
            break;
        case EBUSY:
            ret = ERROR_BUSYBOX_EBUSY;
            break;
        case EFAULT:
            ret = ERROR_BUSYBOX_EFAULT;
            break;
        case EIO:
            ret = ERROR_BUSYBOX_EIO;
            break;
        case EISDIR:
            ret = ERROR_BUSYBOX_EISDIR;
            break;
        case ELOOP:
            ret = ERROR_BUSYBOX_ELOOP;
            break;
        case ENAMETOOLONG:
            ret = ERROR_BUSYBOX_ENAMETOOLONG;
            break;
        case ENOENT:
            ret = ERROR_BUSYBOX_ENOENT;
            break;
        case ENOMEM:
            ret = ERROR_BUSYBOX_ENOMEM;
            break;
        case ENOTDIR:
            ret = ERROR_BUSYBOX_ENOTDIR;
            break;
        case EPERM:
            ret = ERROR_BUSYBOX_EPERM;
            break;
        case EROFS:
            ret = ERROR_BUSYBOX_EROFS;
            break;
        case EBADF:
            ret = ERROR_BUSYBOX_EBADF;
            break;
        case EINVAL:
            ret = ERROR_BUSYBOX_EINVAL;
            break;
        case ESRCH:
            ret = ERROR_BUSYBOX_ESRCH;
            break;
        case ENOSPC:
            ret = ERROR_BUSYBOX_ENOSPC;
            break;
        case EWOULDBLOCK:
            ret = ERROR_BUSYBOX_EWOULDBLOCK;
            break;
        case ETXTBSY:
            ret = ERROR_BUSYBOX_ETXTBSY;
            break;
        case EOVERFLOW:
            ret = ERROR_BUSYBOX_EOVERFLOW;
            break;
        case EOPNOTSUPP:
            ret = ERROR_BUSYBOX_EOPNOTSUPP;
            break;
        case ENXIO:
            ret = ERROR_BUSYBOX_ENXIO;
            break;
        case ENODEV:
            ret = ERROR_BUSYBOX_ENODEV;
            break;
        case ENFILE:
            ret = ERROR_BUSYBOX_ENFILE;
            break;
        case EDQUOT:
            ret = ERROR_BUSYBOX_EDQUOT;
            break;
        case EEXIST:
            ret = ERROR_BUSYBOX_EEXIST;
            break;
        case EFBIG:
            ret = ERROR_BUSYBOX_EFBIG;
            break;
        case EINTR:
            ret = ERROR_BUSYBOX_EINTR;
            break;
        case EMFILE:
            ret = ERROR_BUSYBOX_EMFILE;
            break;
        default:
            ret = ERROR_BUSYBOX_UNKONOWN;
            break;
    }

    return ret;
}

bool wildcard_cmp(const char *text, const char *pattern)
{
    if(text == NULL)
        return false;

    if(pattern == NULL)
        return false;

    while((pattern[0] != '\0') && (text[0] != '\0'))
    {
        if(pattern[0] == '?')
        {
            if(!text[0])
                return false;

            if(pattern[1] == '?')
            {
                pattern += 2;
            }
            else
            {
                if(text[0] == '/')
                    return false;

                ++pattern;
            }

            ++text;
        }
        else if(pattern[0] == '*')
        {
            int allow_slash = pattern[1] == '*';

            while(pattern[0] == '*')
            {
                ++pattern;
            }

            if(allow_slash && pattern[0] == '\0')
                return true;

            do
            {
                if(wildcard_cmp(text, pattern))
                    return true;
            } while((text[0] != '\0') && (*text++ != '/' || allow_slash));
        }
        else
        {
            if(text[0] != pattern[0])
                return false;

            ++pattern;
            ++text;
        }
    }

    return *pattern == *text;
}


struct env_param * get_env(struct env_param *head, const char *key)
{
    struct env_param *env = head;

    if(head == NULL)
        return NULL;

    while(env)
    {
        if(env->key)
        {
            if(strcmp(env->key, key) == 0)
                break;
        }
        env = env->next;
    }

    return env;
}

struct env_param *
add_env(struct env_param **head, const char *key, const char *value)
{
    struct env_param *env = NULL;
    struct env_param *tmp_env = NULL;

    if((key == NULL) || (value == NULL))
        return NULL;

    if(*head)
    {
        env = get_env(*head, key);
        if(env)
        {
            if(env->value)
                free(env->value);
            env->value = strdup(value);
        }
    }

    if(env == NULL)
    {
        env = calloc(sizeof(struct env_param), 1);

        if(*head == NULL)
            *head = env;
        else
        {
            tmp_env = *head;
            while(tmp_env->next)
                tmp_env = tmp_env->next;
            tmp_env->next = env;
        }
        env->key = strdup(key);
        env->value = strdup(value);
    }

    return env;
}

bool remove_env(struct env_param **head, const char *key)
{
    struct env_param *env = *head;
    struct env_param *tmp_env = *head;
    bool ret = false;

    if((*head == NULL) || (key == NULL))
        return false;

    while(env)
    {
        if(env->key)
        {
            if(strcmp(env->key, key) == 0)
            {
                if(env == *head)
                    *head = env->next;
                else
                    tmp_env->next = env->next;

                if(env->key)
                    free(env->key);
                if(env->value)
                    free(env->value);
                free(env);

                ret = true;
                break;
            }
        }
        tmp_env = env;
        env = env->next;
    }

    return ret;
}

struct env_param *
modify_env(struct env_param **head, const char *key, const char *value)
{
    struct env_param *env = NULL;

    if((key == NULL) || (value == NULL))
        return NULL;

    env = get_env(*head, key);
    if(env == NULL)
        env = add_env(head, key, value);
    else
    {
        if(env->value)
            free(env->value);
        env->value = strdup(value);
    }

    return env;
}

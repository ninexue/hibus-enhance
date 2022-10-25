#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/vfs.h>

#include <hibus.h>
#include <hibox/json.h>

#include "hibus_busybox.h"
#include "interface.h"
#include "helper.h"
#include "system.h"

extern const char *op_errors[];

static struct busybox_procedure sys_procedure[] =
{
    {METHOD_HIBUS_BUSYBOX_CHDIR,    changeDirectory},
    {METHOD_HIBUS_BUSYBOX_DF,       getDiskInfo},
    {METHOD_HIBUS_BUSYBOX_SETENV,   hibus_setenv},
    {METHOD_HIBUS_BUSYBOX_UNSETENV, hibus_unsetenv},
    {METHOD_HIBUS_BUSYBOX_GETENV,   hibus_getenv}
};

static struct busybox_event sys_event [] = {};


char * changeDirectory(hibus_conn* conn, const char* from_endpoint,
                const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    uid_t euid;
    const char *path = NULL;
    char dirname[PATH_MAX] = {0, };
    struct stat dict_stat;
    hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_CHDIR,
                                strlen(METHOD_HIBUS_BUSYBOX_CHDIR)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    euid = json_object_get_int(jo_tmp);
    change_euid(from_endpoint, euid);

    // get working directory
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    path = json_object_get_string(jo_tmp);
    if(path == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    if(path[0] != '/')
    {
        if(user->cwd[0] == '/')
        {
            strcat(dirname, user->cwd);
            strcat(dirname, "/");
            strcat(dirname, path);
        }
        else
        {
            ret_code = ERROR_BUSYBOX_WORKING_DIRECTORY;
            goto failed;
        }
    }
    else
        strcpy(dirname, path);

    if(stat(dirname, &dict_stat) < 0)
    {
           ret_code = ERROR_BUSYBOX_ENOENT;
        goto failed;
    }

    if(!S_ISDIR(dict_stat.st_mode))
    {
           ret_code = ERROR_BUSYBOX_ENOTDIR;
        goto failed;
    }

    strcpy(user->cwd, dirname);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}",
            ret_code, op_errors[-1 * ret_code]);

    restore_euid();

    return ret_string;
}


char * getDiskInfo(hibus_conn* conn, const char* from_endpoint,
                const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(256);
    int ret_code = ERROR_BUSYBOX_OK;
    const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    struct statfs diskInfo;
    uid_t euid;
    unsigned long long blocksize = 0;
    unsigned long long totalsize = 0;
    unsigned long long freeDisk = 0;
    unsigned long long availableDisk = 0;
    hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);


    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_DF,
                                strlen(METHOD_HIBUS_BUSYBOX_DF)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    euid = json_object_get_int(jo_tmp);
    change_euid(from_endpoint, euid);

    // get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    path = json_object_get_string(jo_tmp);
    if(path == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    if(path[0] != '/')
    {
        if(user->cwd[0] != '/')
        {
            ret_code = ERROR_BUSYBOX_WORKING_DIRECTORY;
            goto failed;
        }
        else
        {
            strcpy(full_path, user->cwd);
            strcat(full_path, "/");
            strcat(full_path, path);
        }
    }
    else
        strcpy(full_path, path);

    statfs(full_path, &diskInfo);
    blocksize = diskInfo.f_bsize;
    totalsize = blocksize * diskInfo.f_blocks;
    freeDisk = diskInfo.f_bfree * blocksize;
    availableDisk = blocksize * diskInfo.f_bavail;

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
            "{\"blocksize\":%lld, \"totalsize\": %lld, \"freeDisk\": %lld, "
            "\"availableDisk\": %lld, \"errCode\":%d, \"errMsg\":\"%s\"}",
            blocksize, totalsize, freeDisk, availableDisk,
            ret_code, op_errors[-1 * ret_code]);

    restore_euid();

    return ret_string;
}

char * hibus_setenv(hibus_conn* conn, const char* from_endpoint,
                const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    uid_t euid;
    const char *envName = NULL;
    const char *envValue = NULL;
    struct env_param *env = NULL;
    hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_SETENV,
                                strlen(METHOD_HIBUS_BUSYBOX_SETENV)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    euid = json_object_get_int(jo_tmp);
    change_euid(from_endpoint, euid);

    // get env name
    if(json_object_object_get_ex(jo, "envName", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    envName = json_object_get_string(jo_tmp);
    if(envName == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    // get env value
    if(json_object_object_get_ex(jo, "envValue", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    envValue = json_object_get_string(jo_tmp);
    if(envValue == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    env = add_env(&(user->env), envName, envValue);
    if(env == NULL)
        ret_code = ERROR_BUSYBOX_INSUFFICIENT_MEM;
    else
    {
        if(setenv(envName, envValue, 1))
            ret_code = get_error_code();
    }

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}",
            ret_code, op_errors[-1 * ret_code]);

    restore_euid();

    return ret_string;
}

char * hibus_unsetenv(hibus_conn* conn, const char* from_endpoint,
                const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    uid_t euid;
    const char *envName = NULL;
    bool ret = false;
    hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_UNSETENV,
                                strlen(METHOD_HIBUS_BUSYBOX_UNSETENV)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    euid = json_object_get_int(jo_tmp);
    change_euid(from_endpoint, euid);

    // get env name
    if(json_object_object_get_ex(jo, "envName", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    envName = json_object_get_string(jo_tmp);
    if(envName == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    ret = remove_env(&(user->env), envName);
    if(!ret)
        ret_code = ERROR_BUSYBOX_INSUFFICIENT_MEM;
    else
    {
        if(unsetenv(envName))
            ret_code = get_error_code();
    }

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}",
            ret_code, op_errors[-1 * ret_code]);

    restore_euid();

    return ret_string;
}

char * hibus_getenv(hibus_conn* conn, const char* from_endpoint,
                const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    uid_t euid;
    const char *envName = NULL;
    struct env_param *env = NULL;
    hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_GETENV,
                                strlen(METHOD_HIBUS_BUSYBOX_GETENV)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    euid = json_object_get_int(jo_tmp);
    change_euid(from_endpoint, euid);

    // get env name
    if(json_object_object_get_ex(jo, "envName", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
    envName = json_object_get_string(jo_tmp);
    if(envName == NULL)
    {
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

    env = get_env(user->env, envName);

failed:
    if(jo)
        json_object_put (jo);

    if(env)
        sprintf(ret_string, "{\"value\":\"%s\", \"errCode\":%d, \"errMsg\":\"%s\"}",
                env->value, ret_code, op_errors[-1 * ret_code]);
    else
        sprintf(ret_string, "{\"value\":\"\", \"errCode\":%d, \"errMsg\":\"%s\"}",
                ret_code, op_errors[-1 * ret_code]);

    restore_euid();

    return ret_string;
}

void sys_register(hibus_conn *context)
{
    size_t i = 0;
    size_t size = sizeof(sys_procedure) / sizeof(struct busybox_procedure);
    int ret_code = 0;

    for(i = 0; i < size; i++)
    {
        ret_code = hibus_register_procedure(context, sys_procedure[i].name,
                                        NULL, NULL, sys_procedure[i].handler);
        if(ret_code)
        {
            fprintf(stderr, "Busybox--system: Error for register procedure"
                            "%s, %s.\n", sys_procedure[i].name,
                            hibus_get_err_message(ret_code));
        }
    }

    size = sizeof(sys_event) / sizeof(struct busybox_event);
    for(i = 0; i < size; i++)
    {
        ret_code = hibus_register_event(context, sys_event[i].name, NULL, NULL);
        if(ret_code)
        {
            fprintf(stderr, "Busybox--system: Error for register event"
                            "%s, %s.\n",
                            sys_event[i].name, hibus_get_err_message(ret_code));
        }
    }
}

void sys_revoke(hibus_conn *context)
{
    size_t i = 0;
    size_t size = sizeof(sys_procedure) / sizeof(struct busybox_procedure);

    for(i = 0; i < size; i++)
        hibus_revoke_procedure(context, sys_procedure[i].name);

    size = sizeof(sys_event) / sizeof(struct busybox_event);
    for(i = 0; i < size; i++)
        hibus_revoke_event(context, sys_event[i].name);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#include <hibus.h>

#include "hibus_busybox.h"

#define SOCKET_PATH         "/var/tmp/hibus.sock"
#define APP_NAME_SUMMER     "cn.summer.hzy"
#define RUNNER_NAME_TEST    "test"
#define RUNNER_NAME_BUSYBOX "busybox"

static int
setenv_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= setenv ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
getenv_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;
    const char *value = NULL;

    fprintf(stderr, "============= getenv ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get env
    if(json_object_object_get_ex(jo, "value", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get ENV value error.\n\n\n");
        return -1;
    }
    value = json_object_get_string(jo_tmp);

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);


    fprintf(stderr, "ENV TEST value is %s\n", value);
    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
df_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    unsigned long long blocksize = 0;
    unsigned long long totalsize = 0;
    unsigned long long freeDisk = 0;
    unsigned long long availableDisk = 0;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= df ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get block size
    if(json_object_object_get_ex(jo, "blocksize", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get blocksize error.\n\n\n");
        return -1;
    }
    blocksize = json_object_get_uint64(jo_tmp);

    // get total size
    if(json_object_object_get_ex(jo, "totalsize", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get totalsize error.\n\n\n");
        return -1;
    }
    totalsize = json_object_get_uint64(jo_tmp);

    // get freeDisk
    if(json_object_object_get_ex(jo, "freeDisk", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get freeDisk error.\n\n\n");
        return -1;
    }
    freeDisk = json_object_get_uint64(jo_tmp);

    // get availableDisk
    if(json_object_object_get_ex(jo, "availableDisk", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get availableDisk error.\n\n\n");
        return -1;
    }
    availableDisk = json_object_get_uint64(jo_tmp);

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);


    fprintf(stderr, "blocksize = %lld\n", blocksize);
    fprintf(stderr, "totalsize = %lld\n", totalsize);
    fprintf(stderr, "freeDisk = %lld\n", freeDisk);
    fprintf(stderr, "availableDisk = %lld\n", availableDisk);
    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
chdir_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= chdir ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
ls_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    json_object *object = NULL;
    json_object *list = NULL;
    int errCode = 0;
    const char *errMsg = NULL;
    size_t i = 0;
    size_t size = 0;

    fprintf(stderr, "=============== ls ==============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 10);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    // get list
    if(json_object_object_get_ex(jo, "list", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get list error.\n\n\n");
        return -1;
    }
    size = json_object_array_length(jo_tmp);
    list = jo_tmp;

    fprintf(stderr, "total %ld\n", size);
    for(i = 0; i < size; i++)
    {
        object = json_object_array_get_idx(list, i);
        if(object)
        {
            json_object_object_get_ex(object, "type", &jo_tmp);
            fprintf(stderr, "%s", json_object_get_string(jo_tmp));

            json_object_object_get_ex(object, "mode", &jo_tmp);
            fprintf(stderr, "%s", json_object_get_string(jo_tmp));

            json_object_object_get_ex(object, "link", &jo_tmp);
            fprintf(stderr, "\t%d", json_object_get_int(jo_tmp));

            json_object_object_get_ex(object, "uid", &jo_tmp);
            fprintf(stderr, "\t%d", json_object_get_int(jo_tmp));

            json_object_object_get_ex(object, "gid", &jo_tmp);
            fprintf(stderr, "\t%d", json_object_get_int(jo_tmp));

            json_object_object_get_ex(object, "size", &jo_tmp);
            fprintf(stderr, "\t%ld", (long int)json_object_get_int(jo_tmp));

            json_object_object_get_ex(object, "ctime", &jo_tmp);
            fprintf(stderr, "\t%s", json_object_get_string(jo_tmp));

            json_object_object_get_ex(object, "name", &jo_tmp);
            fprintf(stderr, "\t%s\n", json_object_get_string(jo_tmp));
        }
    }

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
touch_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= touch ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
unlink_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= unlink ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
mkdir_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= mkdir ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
rm_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "=============== rm ==============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

static int
rmdir_handler(hibus_conn* conn, const char* from_endpoint,
            const char* method_name, const char *call_id,
            int ret_code, const char* ret_value)
{
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    int errCode = 0;
    const char *errMsg = NULL;

    fprintf(stderr, "============= rmdir ============\n");

    // analyze json
    jo = hibus_json_object_from_string(ret_value, strlen(ret_value), 2);
    if(jo == NULL)
    {
        fprintf(stderr, "Json syntax error.\n\n\n");
        return -1;
    }

    // get error code
    if(json_object_object_get_ex(jo, "errCode", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errCode error.\n\n\n");
        return -1;
    }
    errCode = json_object_get_int(jo_tmp);

    // get error message
    if(json_object_object_get_ex(jo, "errMsg", &jo_tmp) == 0)
    {
        fprintf(stderr, "Get errMsg error.\n\n\n");
        return -1;
    }
    errMsg = json_object_get_string(jo_tmp);

    fprintf(stderr, "return code = %d\n", ret_code);
    fprintf(stderr, "error  code = %d\n", errCode);
    fprintf(stderr, "error message: %s\n", errMsg);

    fprintf(stderr, "\n\n\n");

    if(jo)
        json_object_put (jo);

    return 0;
}

int main(void)
{
    int ret_code = 0;
    int fd_hibus = -1;
    hibus_conn * hibus_context = NULL;
    char *endpoint = NULL;
    const char *call_id = NULL;
    uid_t euid = geteuid();
    char command[100];
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;


    fd_hibus = hibus_connect_via_unix_socket(SOCKET_PATH, APP_NAME_SUMMER,
                                            RUNNER_NAME_TEST, &hibus_context);
    if((fd_hibus <= 0) || (hibus_context == NULL))
    {
        fprintf(stderr, "Connects to HIBUS server error, %s.\n",
                                            hibus_get_err_message(fd_hibus));
        exit(1);
    }

    endpoint = hibus_assemble_endpoint_name_alloc(HIBUS_LOCALHOST,
                    APP_NAME_SUMMER, RUNNER_NAME_BUSYBOX);

    // chdir to inexistent directory
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/abcd\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_CHDIR, command, 1000,
                    chdir_handler, &call_id);

    // chdir to $HOME, it will be successufully.
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_CHDIR, command, 1000,
                    chdir_handler, &call_id);

    // get disk information
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_DF, command, 1000,
                    df_handler, &call_id);

    // ls to only list visible files in $HOME
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // ls to list all files in $HOME
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\", \"option\":\"a\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // mkdir for test with absolute path: $HOME/busybox1
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_MKDIR, command, 1000,
                    mkdir_handler, &call_id);

    // ls to only list visible files in $HOME
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // mkdir for test with relative path: $HOME/busybox2
    sprintf(command, "{\"euid\":%u, \"path\":\"testbusybox1\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_MKDIR, command, 1000,
                    mkdir_handler, &call_id);

    // ls to only list visible files in $HOME
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/a.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // rmdir
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_RMDIR, command, 1000,
                    rmdir_handler, &call_id);

    // rmdir
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox1\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_RMDIR, command, 1000,
                    rmdir_handler, &call_id);

    // ls to only list visible files in $HOME
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // unlink
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/a.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_UNLINK, command, 1000,
                    unlink_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/a.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/b.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/c.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/a.cpp\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/b.cpp\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // touch
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/c.cpp\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_TOUCH, command, 1000,
                    touch_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/*.c\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox/*.cpp\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // rm
    sprintf(command, "{\"euid\":%u, \"path\":\"%s/testbusybox\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_RM, command, 1000,
                    rm_handler, &call_id);

    // ls to only list visible files in $HOME/testbusybox
    sprintf(command, "{\"euid\":%u, \"path\":\"%s\"}", euid, homedir);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_LS, command, 1000,
                    ls_handler, &call_id);

    // set env
    sprintf(command, "{\"euid\":%u, \"envName\":\"TEST\", \"envValue\":"
                     "\"HIBUS\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_SETENV, command, 1000,
                    setenv_handler, &call_id);

    // get env
    sprintf(command, "{\"euid\":%u, \"envName\":\"TEST\"}", euid);
    ret_code = hibus_call_procedure(hibus_context, endpoint,
                    METHOD_HIBUS_BUSYBOX_GETENV, command, 1000,
                    getenv_handler, &call_id);

    while(1)
    {
        ret_code = hibus_wait_and_dispatch_packet(hibus_context, 1000);
        if(ret_code && (ret_code != HIBUS_EC_TIMEOUT))
            fprintf(stderr, "For hibus_wait_and_dispatch_packet, %s.\n",
                                        hibus_get_err_message(ret_code));
    }

    free(endpoint);
    hibus_disconnect(hibus_context);

    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <net/if.h>
#include <sys/timerfd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <arpa/inet.h>
#include <errno.h>

#include <hibus.h>
#include <hibox/json.h>

#include "hibus_busybox.h"
#include "interface.h"
#include "helper.h"
#include "filesystem.h"

extern const char *op_errors[];

static struct busybox_procedure fs_procedure[] =
{
	{METHOD_HIBUS_BUSYBOX_LS, 		listDirectory},
	{METHOD_HIBUS_BUSYBOX_RM, 		removeFile},
	{METHOD_HIBUS_BUSYBOX_RMDIR, 	removeDirectory},
	{METHOD_HIBUS_BUSYBOX_MKDIR, 	makeDirectory},
	{METHOD_HIBUS_BUSYBOX_UNLINK, 	unlinkFile},
	{METHOD_HIBUS_BUSYBOX_TOUCH, 	touchFile}
};

static struct busybox_event fs_event [] = {};


char * listDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(8192);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    sprintf(ret_string, "{\"data\":[");

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_LS,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);
/*
    const char * device_name = NULL;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    memset(ret_string, 0, 8192);
    sprintf(ret_string, "{\"data\":[");

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_START_SCAN, strlen(METHOD_WIFI_START_SCAN)))
    {
        ret_code = ERR_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERR_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "device", &jo_tmp) == 0)
    {
        ret_code = ERR_WRONG_JSON;
        goto failed;
    }

    device_name = json_object_get_string(jo_tmp);
    if(device_name && strlen(device_name) == 0)
    {
        ret_code = ERR_NO_DEVICE_NAME_IN_PARAM;
        goto failed;
    }

    // device does exist?
    index = get_device_index(device, device_name);
    if(index == -1)
    {
        ret_code = ERR_NO_DEVICE_IN_SYSTEM;
        goto failed;
    }

    if(device[index].type != DEVICE_TYPE_WIFI)
    {
        ret_code = ERR_NOT_WIFI_DEVICE;
        goto failed;
    }
        
    // whether library has been loaded
    if(device[index].lib_handle == NULL)
    {
        ret_code = ERR_LOAD_LIBRARY;
        goto failed;
    }

    wifi_device = (WiFi_device *)device[index].device;
    if(wifi_device->context == NULL)
    {
        ret_code = ERR_DEVICE_NOT_OPENNED;
        goto failed;
    }

    if((device[index].status == DEVICE_STATUS_DOWN) || (device[index].status == DEVICE_STATUS_UNCERTAIN))
    {
        ret_code = ERR_OPEN_WIFI_DEVICE;
        if(ifconfig_helper(device_name, 1))
            goto failed;
    }

    ret_code = wifi_device->wifi_device_Ops->start_scan(wifi_device->context);

    pthread_mutex_lock(&(wifi_device->list_mutex));
    wifi_hotspot * node = wifi_device->first_hotspot;
    while(node)
    {
        if(node != wifi_device->first_hotspot)
            sprintf(ret_string + strlen(ret_string), ",");
        sprintf(ret_string + strlen(ret_string), 
                "{"
                "\"bssid\":\"%s\","
                "\"ssid\":\"%s\","
                "\"frequency\":\"%s\","
                "\"capabilities\":\"%s\","
                "\"signalStrength\":%d,"
                "\"isConnected\":%s"
                "}",
                node->bssid, node->ssid, node->frenquency, node->capabilities, node->signal_strength, node->isConnect ? "true": "false");

        node = node->next;
    }
    pthread_mutex_unlock(&(wifi_device->list_mutex));

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string), "],\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string), "],\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;

}

char * removeFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RM,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);


failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * removeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RMDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);



failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * makeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_MKDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);


failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * unlinkFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_UNLINK,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);



failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


char * touchFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_TOUCH,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
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

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


void fs_register(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);
    int ret_code = 0;

	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_procedure(context, fs_procedure[i].name,
										NULL, NULL, fs_procedure[i].handler);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register procedure"
							"%s, %s.\n", fs_procedure[i].name,
							hibus_get_err_message(ret_code));
	        return;
    	}
	}

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_event(context, fs_event[i].name, NULL, NULL);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register event"
							"%s, %s.\n",
							fs_event[i].name, hibus_get_err_message(ret_code));
	        return;
    	}
	}
}

void fs_revoke(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);

	for(i = 0; i < size; i++)
    	hibus_revoke_procedure(context, fs_procedure[i].name);

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
		hibus_revoke_event(context, fs_event[i].name);

}

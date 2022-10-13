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

#include "filesystem.h"
#include "hibus_busybox.h"

extern const char *op_errors[];
extern hibus_conn * hibus_context_inetd;

char * listDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(8192);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
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
    return ret_string;

}

char * removeFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(4096);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    const char * device_name = NULL;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_STOP_SCAN, strlen(METHOD_WIFI_STOP_SCAN)))
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

    ret_code = wifi_device->wifi_device_Ops->stop_scan(wifi_device->context);

failed:
    if(jo)
        json_object_put (jo);

    memset(ret_string, 0, 4096);
    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/
    return ret_string;
}

char * removeDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(4096);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    const char * device_name = NULL;
    const char * ssid = NULL;
    const char * password = NULL;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_CONNECT_AP, strlen(METHOD_WIFI_CONNECT_AP)))
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
    
    // get WiFi ssid 
    if(json_object_object_get_ex(jo, "ssid", &jo_tmp) == 0)
    {
        ret_code = ERR_WRONG_JSON;
        goto failed;
    }

    ssid = json_object_get_string(jo_tmp);
    if(ssid && strlen(ssid) == 0)
    {
        ret_code = ERR_NO_DEVICE_NAME_IN_PARAM;
        goto failed;
    }

    // get WiFi password 
    if(json_object_object_get_ex(jo, "password", &jo_tmp) == 0)
    {
        ret_code = ERR_WRONG_JSON;
        goto failed;
    }

    password = json_object_get_string(jo_tmp);
    if(password && strlen(password) == 0)
    {
        ret_code = ERR_NO_DEVICE_NAME_IN_PARAM;
        goto failed;
    }

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

    ret_code = wifi_device->wifi_device_Ops->connect(wifi_device->context, ssid, password);

    if(ret_code == 0)
    {
        char reply[512];
        int reply_length = 512;
        int i = 0;

        memset(wifi_device->bssid, 0, HOTSPOT_STRING_LENGTH);

        ret_code = ERR_LIBRARY_OPERATION;
        while(ret_code)
        {
            usleep(100000);

            i ++;
            if(i > 200)
                goto failed;

            memset(reply, 0, 512);
            ret_code = wifi_device->wifi_device_Ops->get_cur_net_info(wifi_device->context, reply, reply_length);
        }

        if(ret_code == 0)
        {
            char * tempstart = NULL;
            char * tempend = NULL;
            char content[64];

            memset(content, 0, 64);
            tempstart = strstr(reply, "wpa_state=");
            if(tempstart)
            {
                tempstart += strlen("wpa_state=");
                tempend = strstr(tempstart, "\n");
                if(tempend)
                {
                    memcpy(content, tempstart, tempend - tempstart);
                    if(strncasecmp(content, "COMPLETED", strlen("COMPLETED")) == 0)
                    {
                        device[index].status = DEVICE_STATUS_RUNNING;
                        
                        // bssid
                        tempstart = strstr(reply, "bssid=");
                        if(tempstart)
                        {
                            tempstart += strlen("bssid=");
                            tempend = strstr(tempstart, "\n");
                            if(tempend)
                                memcpy(wifi_device->bssid, tempstart, tempend - tempstart);
                        }
                    }
                }
            }

            wifi_hotspot * node = wifi_device->first_hotspot;

            pthread_mutex_lock(&(wifi_device->list_mutex));
            while(node)
            {
                if(strcmp((char *)wifi_device->bssid, (char *)node->bssid) == 0)
                {
                    wifi_device->signal = node->signal_strength;
                    break;
                }
                node = node->next;
            }
            pthread_mutex_unlock(&(wifi_device->list_mutex));
        }
    }
failed:
    if(jo)
        json_object_put (jo);

    memset(ret_string, 0, 4096);
    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/
    return ret_string;
}

char * makeDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(4096);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    const char * device_name = NULL;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_DISCONNECT_AP, strlen(METHOD_WIFI_CONNECT_AP)))
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

    wifi_device->wifi_device_Ops->disconnect(wifi_device->context);

    memset(wifi_device->bssid, 0, HOTSPOT_STRING_LENGTH);
    wifi_device->signal = 0;

    // remove hot spots list
    wifi_hotspot * node = NULL;
    wifi_hotspot * tempnode = NULL;

    pthread_mutex_lock(&(wifi_device->list_mutex));
    node = wifi_device->first_hotspot;
    while(node)
    {
        tempnode = node->next;
        free(node);
        node = tempnode;
    }
    wifi_device->first_hotspot = NULL;
    pthread_mutex_unlock(&(wifi_device->list_mutex));

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/
    return ret_string;
}

char * unlinkFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(4096);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    const char * device_name = NULL;
    char reply[512];
    int reply_length = 512;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    memset(ret_string, 0, 4096);
    sprintf(ret_string, "{\"data\":{");

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_GET_NETWORK_INFO, strlen(METHOD_WIFI_GET_NETWORK_INFO)))
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

    ret_code = wifi_device->wifi_device_Ops->get_cur_net_info(wifi_device->context, reply, reply_length);


    if(ret_code == 0)
    {
        char * tempstart = NULL;
        char * tempend = NULL;
        char content[64];

        memset(content, 0, 64);
        tempstart = strstr(reply, "wpa_state=");
        if(tempstart)
        {
            tempstart += strlen("wpa_state=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
            {
                memcpy(content, tempstart, tempend - tempstart);
                if(strncasecmp(content, "COMPLETED", strlen("COMPLETED")))
                {
                    ret_code = ERR_DEVICE_NOT_CONNECT;
                    goto failed;
                }
            }
            else
            {
                ret_code = ERR_DEVICE_NOT_CONNECT;
                goto failed;
            }
        }
        else
        {
            ret_code = ERR_DEVICE_NOT_CONNECT;
            goto failed;
        }
        
        device[index].status = DEVICE_STATUS_RUNNING;

        // device name
        sprintf(ret_string + strlen(ret_string), "\"device\":\"%s\",", device_name);

        // bssid
        memset(content, 0, 64);
        tempstart = strstr(reply, "bssid=");
        if(tempstart)
        {
            tempstart += strlen("bssid=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"bssid\":\"%s\",", content);
        memset(wifi_device->bssid, 0, HOTSPOT_STRING_LENGTH);
        sprintf(wifi_device->bssid, "%s", content);

        // frenquency 
        memset(content, 0, 64);
        tempstart = strstr(tempend, "freq=");
        if(tempstart)
        {
            tempstart += strlen("freq=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"frenquency\":\"%s MHz\",", content);

        // ssid
        memset(content, 0, 64);
        tempstart = strstr(tempend, "ssid=");
        if(tempstart)
        {
            tempstart += strlen("ssid=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"ssid\":\"%s\",", content);


        // encryptionType
        memset(content, 0, 64);
        tempstart = strstr(tempend, "key_mgmt=");
        if(tempstart)
        {
            tempstart += strlen("key_mgmt=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"encryptionType\":\"%s\",", content);

        // ip
        memset(content, 0, 64);
        tempstart = strstr(tempend, "ip_address=");
        if(tempstart)
        {
            tempstart += strlen("ip_address=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"ip\":\"%s\",", content);
        memset(device[index].ip, 0, NETWORK_ADDRESS_LENGTH);
        sprintf(device[index].ip, "%s", content);

        // mac
        memset(content, 0, 64);
        tempstart = strstr(tempend, "address=");
        if(tempstart)
        {
            tempstart += strlen("address=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"mac\":\"%s\",", content);
        memset(device[index].mac, 0, NETWORK_ADDRESS_LENGTH);
        sprintf(device[index].mac, "%s", content);

        // speed
        sprintf(ret_string + strlen(ret_string), "\"speed\":\"%d Mbps\",", device[index].speed);

        // gate way

        // singal
        sprintf(ret_string + strlen(ret_string), "\"signalStrength\":%d", wifi_device->signal);
    }

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string), "},");
    sprintf(ret_string + strlen(ret_string), "\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/
    return ret_string;
}


char * touchFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(4096);
/*
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
    const char * device_name = NULL;
    char reply[512];
    int reply_length = 512;
    int index = -1;
    int ret_code = ERR_NO;
    WiFi_device * wifi_device = NULL;

    memset(ret_string, 0, 4096);
    sprintf(ret_string, "{\"data\":{");

    // get device array
    network_device * device = hibus_conn_get_user_data(conn);
    if(device == NULL)
    {
        ret_code = ERR_NONE_DEVICE_LIST;
        goto failed;
    }

    // get procedure name
    if(strncasecmp(to_method, METHOD_WIFI_GET_NETWORK_INFO, strlen(METHOD_WIFI_GET_NETWORK_INFO)))
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

    ret_code = wifi_device->wifi_device_Ops->get_cur_net_info(wifi_device->context, reply, reply_length);


    if(ret_code == 0)
    {
        char * tempstart = NULL;
        char * tempend = NULL;
        char content[64];

        memset(content, 0, 64);
        tempstart = strstr(reply, "wpa_state=");
        if(tempstart)
        {
            tempstart += strlen("wpa_state=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
            {
                memcpy(content, tempstart, tempend - tempstart);
                if(strncasecmp(content, "COMPLETED", strlen("COMPLETED")))
                {
                    ret_code = ERR_DEVICE_NOT_CONNECT;
                    goto failed;
                }
            }
            else
            {
                ret_code = ERR_DEVICE_NOT_CONNECT;
                goto failed;
            }
        }
        else
        {
            ret_code = ERR_DEVICE_NOT_CONNECT;
            goto failed;
        }
        
        device[index].status = DEVICE_STATUS_RUNNING;

        // device name
        sprintf(ret_string + strlen(ret_string), "\"device\":\"%s\",", device_name);

        // bssid
        memset(content, 0, 64);
        tempstart = strstr(reply, "bssid=");
        if(tempstart)
        {
            tempstart += strlen("bssid=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"bssid\":\"%s\",", content);
        memset(wifi_device->bssid, 0, HOTSPOT_STRING_LENGTH);
        sprintf(wifi_device->bssid, "%s", content);

        // frenquency 
        memset(content, 0, 64);
        tempstart = strstr(tempend, "freq=");
        if(tempstart)
        {
            tempstart += strlen("freq=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"frenquency\":\"%s MHz\",", content);

        // ssid
        memset(content, 0, 64);
        tempstart = strstr(tempend, "ssid=");
        if(tempstart)
        {
            tempstart += strlen("ssid=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"ssid\":\"%s\",", content);


        // encryptionType
        memset(content, 0, 64);
        tempstart = strstr(tempend, "key_mgmt=");
        if(tempstart)
        {
            tempstart += strlen("key_mgmt=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"encryptionType\":\"%s\",", content);

        // ip
        memset(content, 0, 64);
        tempstart = strstr(tempend, "ip_address=");
        if(tempstart)
        {
            tempstart += strlen("ip_address=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"ip\":\"%s\",", content);
        memset(device[index].ip, 0, NETWORK_ADDRESS_LENGTH);
        sprintf(device[index].ip, "%s", content);

        // mac
        memset(content, 0, 64);
        tempstart = strstr(tempend, "address=");
        if(tempstart)
        {
            tempstart += strlen("address=");
            tempend = strstr(tempstart, "\n");
            if(tempend)
                memcpy(content, tempstart, tempend - tempstart);
        }
        sprintf(ret_string + strlen(ret_string), "\"mac\":\"%s\",", content);
        memset(device[index].mac, 0, NETWORK_ADDRESS_LENGTH);
        sprintf(device[index].mac, "%s", content);

        // speed
        sprintf(ret_string + strlen(ret_string), "\"speed\":\"%d Mbps\",", device[index].speed);

        // gate way

        // singal
        sprintf(ret_string + strlen(ret_string), "\"signalStrength\":%d", wifi_device->signal);
    }

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string), "},");
    sprintf(ret_string + strlen(ret_string), "\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);
*/
    return ret_string;
}
void fs_register(hibus_conn *context)
{
    int ret_code = 0;

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_LS, NULL, NULL, listDirectory);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_LS, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_RM, NULL, NULL, removeFile);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_RM, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_RMDIR, NULL, NULL, removeDirectory);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_RMDIR, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_MKDIR, NULL, NULL, makeDirectory);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_MKDIR, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_UNLINK, NULL, NULL, unlinkFile);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_UNLINK, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_procedure(context, METHOD_HIBUS_BUSYBOX_TOUCH, NULL, NULL, touchFile);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register procedure %s, %s.\n", METHOD_HIBUS_BUSYBOX_TOUCH, hibus_get_err_message(ret_code));
        return;
    }
/*
    ret_code = hibus_register_event(context, WIFIHOTSPOTSCHANGED, NULL, NULL);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register event %s, %s.\n", WIFIHOTSPOTSCHANGED, hibus_get_err_message(ret_code));
        return;
    }

    ret_code = hibus_register_event(context, WIFISIGNALSTRENGTHCHANGED, NULL, NULL);
    if(ret_code)
    {
        fprintf(stderr, "WIFI DAEMON: Error for register event %s, %s.\n", WIFISIGNALSTRENGTHCHANGED, hibus_get_err_message(ret_code));
        return;
    }
*/
}

void fs_revoke(hibus_conn *context)
{
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_LS);
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_RM);
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_RMDIR);
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_MKDIR);
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_UNLINK);
    hibus_revoke_procedure(context, METHOD_HIBUS_BUSYBOX_TOUCH);

//    hibus_revoke_event(context, WIFISIGNALSTRENGTHCHANGED);
//    hibus_revoke_event(context, WIFIHOTSPOTSCHANGED);

}

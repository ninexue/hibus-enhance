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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <hibus.h>
#include <hibox/json.h>

#include "wifi_intf.h"
#include "inetd.h"
#include "tools.h"
#include "wifi.h"
#include "mobile.h"
#include "ethernet.h"
#include "common.h"

extern void report_wifi_scan_info(char * device_name, int type, void * hotspots, int number);

//int load_device_library(network_device * device, int device_index, char * lib_name)
int load_device_library(network_device * device)
{
    char library_path[MAX_PATH];
    void * library_handle = NULL;               // handle of loaded library
	char * library_error = NULL;                // the error message during loading

    if(strlen(device->libpath) == 0)
    {   
        fprintf(stderr, "INETD: library path is invalid for device %s!", device->ifname);
        return -1;
    }

    memset(library_path, 0, MAX_PATH);
    sprintf(library_path, "%s/%s", INETD_LIBRARY_PATH, device->libpath);

    if((access(library_path, F_OK)) == -1)   
    {   
        fprintf(stderr, "INETD: library file %s does not exist, ignore it!", library_path);
        return -1;
    }

    library_handle = dlopen(library_path, RTLD_LAZY);  
    if(!library_handle) 
    {
        fprintf (stderr, "INETD: load %s error: %s\n", library_path, dlerror());
        return -1;
    }

    if(device->type == DEVICE_TYPE_WIFI)
    {
	    hiWiFiDeviceOps * (* __wifi_device_ops_get)(void);   // get all invoke functions 
        hiWiFiDeviceOps * wifi_device_Ops = NULL;

        __wifi_device_ops_get = (hiWiFiDeviceOps * (*) (void))dlsym(library_handle, "__wifi_device_ops_get");
        if((library_error = dlerror()) != NULL)
        {
            fprintf(stderr, "INETD: get wifi_init pointer error: %s\n", library_error);
            dlclose(library_handle);
            return -1;
        }
        wifi_device_Ops = __wifi_device_ops_get();

        if(wifi_device_Ops)
        {
            wifi_device_Ops->report_wifi_scan_info = report_wifi_scan_info;

            WiFi_device * wifi_device = (WiFi_device *)device->device; 
            wifi_device->wifi_device_Ops = wifi_device_Ops;
            pthread_mutex_init(&wifi_device->list_mutex, NULL);
            device->lib_handle = library_handle;
        }
        else
        {
            dlclose(library_handle);
            return -1;
        }
    }

    return 0;
}


# Hibus busybox 协议

## busybox 提供给应用的接口

`busybox`只提供远程过程，不提供订阅事件。

### busybox行者提供的远程过程

#### 显示指定目录下的文件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/ls`
- 参数：
   + `path`：指定的目录；
```json
    {
        "path":"full_path",
    }
```
- 返回值：
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 删除文件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/rm`
- 参数：
   + `fileName`：要删除文件的全路径；
```json
    {
        "fileName":"full_path"
    }
```
- 返回值：
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 删除目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/rmdir`
- 参数：
   + `fileName`：要删除目录的全路径；
```json
    {
        "fileName":"full_path"
    }
```
- 返回值：
   + `data`：返回的数据：
     + `device`：网络设备名；
     + `type`：网络设备类型；
     + `status`：网络设备状态；
     + `mac`：网络设备MAC地址；
     + `ip`：网络设备IP地址；
     + `broadcast`：网络设备广播地址；
     + `subnetmask`：网络设备子网掩码；
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    { 
        "data":[
                    {
                        "device":"eth0",
                        "type":"<wifi|ethernet|mobile|lo>",
                        "status":"<down|up|link>",
                        "mac":"AB:CD:EF:12:34:56",
                        "ip":"192.168.1.128",
                        "broadcast":"192.168.1.255",
                        "subnetmask":"155.255.255.0"
                    },
                    {
                         ......
                    }
               ],
        "errCode": 0,
        "errMsg": "OK"
    }
```
如没有查到网络设备，则`data`为空数组。


#### 创建目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/mkdir`
- 参数：
   + `fileName`：要创建目录的全路径；
```json
    {
        "fileName":"full_path"
    }
```
- 返回值：
   + `data`：返回的数据：
     + `bssid`：
     + `ssid`：网络名称；
     + `frequency`：网络频率；
     + `signalStrength`：网络信号强度；
     + `capabilities`：可用的加密方式；
     + `isConnected`：当前是否连接。
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "data": [
                    {
                         "bssid": "f0:b4:29:24:18:eb",
                         "ssid": "fmsoft-dev",
                         "frequency": "2427MHZ",
                         "signalStrength": 65,
                         "capabilities": ["WPA-PSK-CCMP+TKIP","WPA2-PSK-CCMP+TKIP","WPS","ESS"],
                         "isConnected":true
                    },
                    {
                         ......
                    }
                ],
        "errCode":0,
        "errMsg":"OK"
    }
```

如没有网络热点，则`data`为空数组。

该过程将返回扫描结果。热点列表根据信号强度从大到小排列。如有当前连接网络，则改网络排在第一个。


#### 停止网络热点扫描 

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/wifiStopScanHotspots`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "errCode":0,
        "errMsg":"OK"
    }
```

busybox行者将停止后台进行的定时热点扫描操作，这将导致停止发送`WIFIHOTSPOTSCHANGED`和`NETWORKDEVICECHANGED`泡泡。


#### 连接网络热点

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/wifiConnect`
- 参数：
   + `device`：网络设备名称；
   + `ssid`：网络名称；
   + `password`：网络密码；
   + `autoConnect`：网络中断后是否自动连接；
   + `default`：是否设置为默认网络，下次开机时自动连接。
```json
    {
        "device":"device_name",
        "ssid":"fmsoft-dev",
        "password":"hybridos-hibus",
        "autoConnect":true,
        "default":true
    }
```
- 返回值：
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

在当前版本中，没有实现`autoConnect`、`default`对应的功能。


#### 中断网络连接

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/wifiDisconnect`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 获得当前网络详细信息

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/wifiGetNetworkInfo`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `data`：返回的数据：
     + `device`：网络设备名称；
     + `bssid`：BSSID值；
     + `ssid`：网络名称；
     + `encryptionType`：加密方式；
     + `signalStrength`：信号强度；
     + `mac`：MAC地址；
     + `ip`：IP地址；
     + `frenquency`：网络信号频率；
     + `speed`：网络速度；
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    { 
        "data":{
                    "device":"device_name",
                    "bssid":"0c:4b:54:a5:ec:93",
                    "ssid":"fmsoft-dev",
                    "encryptionType":"WPA2",
                    "mac":"AB:CD:EF:12:34:56",
                    "ip":"192.168.1.128",
                    "frenquency":"5 GHz",
                    "signalStrength":65,
                    "speed":"650 Mbps",
                },
        "errCode":0,
        "errMsg":"OK"
    }
```

如没有查到当前网络详细信息，则`data`为空，`errCode`返回错误原因。


### busybox行者提供的可订阅事件

#### 网络设备发生变化 

- 泡泡名称：`NETWORKDEVICECHANGED`
- bubbleData：
   + `device`：网络设备名称；
   + `type`：网络类型；
   + `status`：设备状态。
```json
    { 
        "device":"device_name",
        "type":"<wifi|ethernet|mobile|lo>",
        "status":"<down|up|link>"
    }
```
- 使用描述：
   + 当网络设备工作状态发生变化时，发送此事件。

#### 网络热点列表发生变化

- 泡泡名称：`WIFIHOTSPOTSCHANGED`
- bubbleData：
   + `changed`：原有网络属性发生变化；
   + `missed`：扫描后消失的热点数组；
   + `found`：   扫描后新增加的热点数组；
     + `bssid`：BSSID值；
     + `ssid`：网络名称；
     + `capabilities`：加密方式；
     + `signalStrength`：信号强度，取值范围在0——100之间。
```json
    {
        "found": [    
                    { 
                        "bssid": "f0:b4:29:24:18:eb",
                        "ssid":"fmsoft-dev",
                        "capabilities": ["WPA-PSK-CCMP+TKIP", "WPA2-PSK-CCMP+TKIP", "WPS", "ESS"],
                        "signalStrength":65
                    },
                    {
                        ......
                    }
                ],
        "missed": [
                    {
                        "bssid": "f2:b3:29:24:18:eb"
                    },
                    {
                        ......
                    }
                ],
        "changed": [
                    {
                        "bssid": "f2:b3:29:24:18:eb",
                        "signalStrength":65,
                        "capabilities": ["WPA-PSK-CCMP+TKIP", "WPA2-PSK-CCMP+TKIP", "WPS", "ESS"]
                    },
                    {
                        ......
                    }
                ]
    }
```
- 使用描述：
   + 当调用`openDevice`过程后，busybox行者将定时扫描WiFi热点，并将相邻两次扫描所获得的热点列表差值，通过该事件发送给订阅该事件的行者；
   + 如要获得完整的热点列表，则调用过程`wifiStartScanHotspots`，通过其返回值获得；
   + `changed`数组中的元素，仅返回变化的属性。没有变化的属性并不返回。


#### 当前网络信号强度发生变化

- 泡泡名称：`WIFISIGNALSTRENGTHCHANGED`
- bubbleData：
   + `bssid`：BSSID值；
   + `ssid`：网络SSID；
   + `signalStrength`：网络信号强度，取值范围在0——100之间。
```json
    {
        "bssid":"f0:b4:29:24:18:eb",
        "ssid":"fmsoft-dev",
        "signalStrength":65
    }
```
- 使用描述：
   + 当网络连接成功后，才开始发送该泡泡；当前网络中断后，不会发送该事件。
   + 该事件的发送间隔，由配置文件中的`scan_time`确定。


## busybox行者工作流程

busybox行者工作流程如下：

1. 两次fork操作，成为守护进程；
2. 调用`hibus_connect_via_unix_socket()`，建立与hiBus服务器的连接；
3. 读取配置文件，获得各项参数；
4. 根据配置文件，装载设备引擎。调用`__wifi_device_ops_get()`函数，获取`hiWiFiDeviceOps`结构指针；
5. 调用`hibus_register_procedure()`注册远程过程；
6. 调用`hibus_register_event()`注册可被订阅事件；
7. 调用`hiWiFiDeviceOps->open()`函数，初始化WiFi设备，连接默认网络；
8. 设置超时时间，循环调用`hibus_wait_and_dispatch_packet()`函数：
   1. 根据配置文件，计算时间，定时调用`hiWiFiDeviceOps->start_scan()`、`hiWiFiDeviceOps->get_signal_strength()`函数；
   2. 定时调用`hiWiFiDeviceOps->get_hotspots()`函数，轮询网络状态；
9. 调用`hibus_revoke_event()`撤销事件（用不到）、调用`hibus_revoke_procedure()`撤销远程过程（用不到）；
10. 调用`hibus_disconnect()`中断与hiBus服务器的连接（用不到）；
11. 调用`dlclose()`，关闭设备引擎。

## 设备引擎需要完成的接口

### WiFi设备引擎

设备引擎必须完成如下描述的所有接口，供busybox行者调用。

在头文件 busybox.h 中，有如下声明：

```c
struct _wifi_context;
typedef struct _wifi_context wifi_context;

#define HOTSPOT_STRING_LENGTH 40
typedef struct _wifi_hotspot
{
    char bssid[HOTSPOT_STRING_LENGTH];
    unsigned char ssid[HOTSPOT_STRING_LENGTH];
    char frenquency[HOTSPOT_STRING_LENGTH];
    char capabilities[HOTSPOT_STRING_LENGTH];
    int  signal_strength;
    bool isConnect;
} wifi_hotspot;

typedef struct _hiWiFiDeviceOps
{
    int (* open) (const char * device_name, wifi_context ** context);
    int (* close) (wifi_context * context);
    int (* connect) (wifi_context * context, const char * ssid, const char *password);
    int (* disconnect) (wifi_context * context);
    int (* get_signal_strength) (wifi_context * context);    
    int (* start_scan) (wifi_context * context);
    int (* stop_scan) (wifi_context * context);
    unsigned int (* get_hotspots) (wifi_context * context, wifi_hotspot ** hotspots);
} hiWiFiDeviceOps;
```

#### 函数集的获得

busybox行者首先调用如下函数，获得操作WiFi设备所需的全部函数指针。

```c
const hiWiFiDeviceOps * __wifi_device_ops_get(void);
```
- 参数：
   + 无 
- 返回值：`hiWiFiDeviceOps`结构指针。如返回值为`NULL`，表示该函数执行失败。


#### 打开设备

根据WiFi设备名，完成对该设备的初始化。

```c
int open (const char * device_name, wifi_context ** context);
```

- 参数：
   + `device_name`：网络设备名； 
   + `context`：`* context`为设备引擎工作的上下文结构指针。该结构由设备引擎自行声明及定义。 
- 返回值：
   + `0`：设备正常打开；
   + `-1`：设备操作错误代码。 

#### 关闭设备

关闭WiFi设备，同时完成相关软、硬件的资源回收。

```c
int close(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：设备正常关闭；
   + `-1`：设备操作错误代码。 

#### 连接网络

```c
int connect(wifi_context * context, const char * ssid, const char *password);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `ssid`：网络SSID； 
   + `password`：网络的密码，如为`NULL`，表示不需要密码； 
- 返回值：
   + `0`：连接指定网络成功；
   + `-1`：连接操作错误代码。 

#### 断开网络

```c
int disconnect(wifi_context * context);

```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：断开连接成功；
   + `-1`：断开连接操作错误代码。 

#### 获取WiFi信号强度

```c
int get_signal_strength(wifi_context * context); 
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + 当前网络信号强度，取值范围为0——100。

#### 开始WiFi网络扫描

WiFi网络扫描是一个异步过程。busybox行者调用该函数后立刻返回。设备引擎开始扫描，其扫描结果将通过busybox行者调用`hiWiFiDeviceOps->get_hotspots()`函数返回。

```c
int start_scan(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：开始网络扫描成功；
   + `-1`：设备操作错误代码。 

#### 停止WiFi网络扫描

该函数终止正在进行的WiFi网络扫描过程。

```c
int stop_scan(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：终止网络扫描成功；
   + `-1`：设备操作错误代码。 

#### 获得热点信息

busybox行者定时调用该函数，获得WiFi热点的信息。

```c
unsigned int get_hotspots (wifi_context * context, wifi_hotspot ** hotspots);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `hotspots`：`wifi_hotspot`结构数组头指针的指针； 
- 返回值：
   + `hotspots`数组的个数。

`hotspots`数组空间由设备引擎开辟，由调用者（busybox）释放。

当设备引擎搜索热点完毕后，在`hotspots`数组最后添加一个元素，该元素的`ssid[0] = 0`，表示搜索过程完毕。


## 错误代码表

| 宏定义                      | errCode | errMsg                                   | 备注                     |
| --------------------------- | ------- | ---------------------------------------- | ------------------------ |
| ERR_NO                      | 0       | success                                  | 正常                     |
| ERR_LIBRARY_OPERATION       | -1      | an error ocures in library operation     | 错误发生在工具层         |
| ERR_NONE_DEVICE_LIST        | -2      | can not get devices list.                | 无法获得网络设备列表     |
| ERR_WRONG_PROCEDURE         | -3      | wrong procedure name.                    | 错误的远程调用名称       |
| ERR_WRONG_JSON              | -4      | wrong Json format.                       | 错误的JSON格式           |
| ERR_NO_DEVICE_NAME_IN_PARAM | -5      | can not find device name in param.       | 设备名不在网络设备列表中 |
| ERR_NO_DEVICE_IN_SYSTEM     | -6      | can not find device in system.           | 系统中找不到该设备       |
| ERR_DEVICE_TYPE             | -7      | invalid network device type.             | 错误的网络设备类型       |
| ERR_LOAD_LIBRARY            | -8      | some error in load library.              | 装载动态库失败           |
| ERR_NOT_WIFI_DEVICE         | -9      | device is not WiFi device.               | 该设备并非WiFi设备       |
| ERR_DEVICE_NOT_OPENNED      | -10     | device has not openned.                  | 网络设备还未打开         |
| ERR_OPEN_WIFI_DEVICE        | -11     | an error ocurs in open wifi device.      | 打开WiFi设备错误         |
| ERR_CLOSE_WIFI_DEVICE       | -12     | an error ocurs in close wifi device.     | 关闭WiFi设备错误         |
| ERR_OPEN_ETHERNET_DEVICE    | -13     | an error ocurs in open ethernet device.  | 打开Ethernet设备错误     |
| ERR_CLOSE_ETHERNET_DEVICE   | -14     | an error ocurs in close ethernet device. | 关闭Ethernet设备错误     |
| ERR_OPEN_MOBILE_DEVICE      | -15     | an error ocurs in open mobile device.    | 打开Mobile设备错误       |
| ERR_CLOSE_MOBILE_DEVICE     | -16     | an error ocurs in close mobile device.   | 关闭Mobile设备错误       |
| ERR_DEVICE_NOT_CONNECT      | -17     | device does not connect any network.     | 网络设备未连接           |



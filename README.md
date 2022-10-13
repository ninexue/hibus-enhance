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
		"euid": euid,
        "path":"full_path",
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

#### 删除文件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/rm`
- 参数：
   + `fileName`：要删除文件的全路径；
```json
    {
		"uid": euid,
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
		"uid": euid,
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
		"uid": euid,
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


#### 

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/unlink`
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

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/touch`
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


### 错误代码表

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



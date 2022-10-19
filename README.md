# Hibus busybox 协议

## busybox 提供给应用的接口

`busybox`只提供远程过程，不提供订阅事件。

### busybox行者提供的远程过程

#### 设置工作目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/chdir`
- 参数：
   + `dictName`：要设置的工作目录的全路径；
```json
    {
		"euid": euid,
        "dictName":"full_path"
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

#### 显示指定目录下的文件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/ls`
- 参数：
   + `path`：指定的目录，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "path":"path",
		"option":"a",
    }
```
- 返回值：
   + `list`：返回的数据：
     + `name`：文件名；
     + `dev`：包含文件的设备ID；
     + `inode`：节点号；
     + `type`：文件类型；
     + `mode`：文件模式；
     + `link`：硬链接数量；
     + `uid`：所有者ID；
     + `gid`：所属组ID；
     + `rdev`：设备ID（如是特殊文件）；
     + `size`：文件大小；
     + `blksize`：文件系统每块大小；
     + `blocks`：文件所占块数量；
     + `atime`：文件最后一次访问时间；
     + `mtime`：文件最后一次修改时间；
     + `ctime`：文件创建时间；
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "list":[
                    {
                        "name":"eth0",
                        "dev":100,
                        "inode":5,
                        "type":"b",
                        "mode":"-rw-rw-r--",
                        "link":1,
                        "uid":1000,
                        "gid":1000,
                        "rdev":100,
                        "size":1026,
                        "blksize":512,
                        "blocks":3,
                        "atime":"2022-09-02",
                        "mtime":"2022-09-02",
                        "ctime":"2022-09-02"
                    },
                    {
                         ......
                    }
               ],
        "errCode": 0,
        "errMsg": "OK"
    }
```

#### 删除文件或目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/rm`
- 参数：
   + `fileName`：要删除文件或目录的路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "fileName":"path"
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

#### 删除空目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/rmdir`
- 参数：
   + `fileName`：要删除空目录的路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "fileName":"path"
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

#### 创建目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/mkdir`
- 参数：
   + `fileName`：要创建目录的路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "fileName":"path"
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

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/unlink`
- 参数：
   + `fileName`：要删除文件的路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "fileName":"path"
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

#### 修改文件属性

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/touch`
   + `fileName`：要修改属性的文件路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
		"euid": euid,
        "fileName":"path"
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



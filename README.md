# Hibus busybox 说明

## 编译方法

- 编译hibox
    ```bash
    # git clone https://github.com/FMSoftCN/hibox
    # cd hibox
    # mkdir build
    # cd build
    # cmake ../
    # make
    # sudo make install
    ```
- 编译hibus
    ```bash
    # git clone https://github.com/FMSoftCN/hibus
    # cd hibox
    # mkdir build
    # cd build
    # cmake ../ -DBUILD_APP_AUTH=OFF
    # make
    # sudo make install
    ```
- 创建运行环境
    ```bash
    # cd hibus/tools
    # sudo ./mk_app_keys.sh cn.summer.hzy
    ```
- 编译hibus-busybox
    ```bash
    # git clone git@github.com:ninexue/hibus-enhance.git
    # cd hibus-enhance
    # mkdir build
    # cd build
    # cmake ../ -DBUILD_HIBUS_NATIVE=OFF
    # make
    ```
    编译选项`BUILD_HIBUS_NATIVE`为`ON`，表示所编译的库为`Hibus`的内建库，`APP_NAME`为`cn.fmsoft.hybridos.hibus`。如果为`OFF`，编译为测试库，`APP_NAME`为`cn.summer.hzy`。

## 运行方法

为了测试和调试方便，首先将代码编译为测试库，用下面的方法进行测试和运行：

- 终端窗口1
    ```bash
    # sudo hibusd
    ```
- 终端窗口2
    ```bash
    # cd hibus-enhance/public/test
    # ./busybox-test
    ```
- 终端窗口3
    ```bash
    # cd hibus-enhance/public/test
    # ./test
    ```
## busybox 提供给应用的接口

`busybox`只提供远程过程，不提供订阅事件。

### busybox行者提供的远程过程

#### 设置工作目录

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/chdir`
- 参数：
   + `euid`：调用者的有效用户ID；
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
   + `euid`：调用者的有效用户ID；
   + `path`：指定的目录，如果不是以`/`开头，则认为是相对路径；
   + `option`：`ls`命令参数，目前支持`a`；
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
   + `euid`：调用者的有效用户ID；
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
   + `euid`：调用者的有效用户ID；
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
   + `euid`：调用者的有效用户ID；
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
   + `euid`：调用者的有效用户ID；
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
   + `euid`：调用者的有效用户ID；
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

#### 获取当前文件系统的磁盘空间

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/df`
   + `euid`：调用者的有效用户ID；
   + `fileName`：要修改属性的文件路径，如果不是以`/`开头，则认为是相对路径；
```json
    {
        "euid": euid,
        "fileName":"path"
    }
```
- 返回值：
   + `blocksize`: 磁盘块大小；
   + `totalsize`: 分区占用块数；
   + `freeDisk`: 空闲空间；
   + `availableDisk`: 可使用的空间；
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "blocksize": 512
        "totalsize": 123456789
        "freeDisk": 2333333
        "availableDisk": 2444444
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 获取环境变量

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/getenv`
   + `euid`：调用者的有效用户ID；
   + `envName`：要获取环境变量的`key`；
```json
    {
        "euid": euid,
        "envName":"key"
    }
```
- 返回值：
   + `value`: 环境变量；
   + `errCode`：返回错误编码，见附表；
   + `errMsg`：错误信息。
```json
    {
        "value": "value"
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 设置环境变量

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/setenv`
   + `euid`：调用者的有效用户ID；
   + `envName`：要设置环境变量的`key`；
   + `envValue`：要设置环境变量的`value`；
```json
    {
        "euid": euid,
        "envName":"key"
        "envValue":"value"
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

#### 删除环境变量

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/busybox/unsetenv`
   + `euid`：调用者的有效用户ID；
   + `envName`：要删除的环境变量的`key`；
```json
    {
        "euid": euid,
        "envName":"key"
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

| 宏定义                          | errCode | errMsg                                        |
| ------------------------------- | ------- | --------------------------------------------- |
| ERROR_BUSYBOX_OK                | 0       | Success.                                      |
| ERROR_BUSYBOX_EACCES            | -1      | Permission denied.                            |
| ERROR_BUSYBOX_EBUSY             | -2      | Device is busy.                               |
| ERROR_BUSYBOX_EFAULT            | -3      | Bad address.                                  |
| ERROR_BUSYBOX_EIO               | -4      | Input/output error.                           |
| ERROR_BUSYBOX_EISDIR            | -5      | Is a directory.                               |
| ERROR_BUSYBOX_ELOOP             | -6      | Too many levels of symbolic links.            |
| ERROR_BUSYBOX_ENAMETOOLONG      | -7      | File name is too long.                        |
| ERROR_BUSYBOX_ENOENT            | -8      | No such file or directory.                    |
| ERROR_BUSYBOX_ENOMEM            | -9      | Insufficient kernel memory was available.     |
| ERROR_BUSYBOX_ENOTDIR           | -10     | Is not a directory.                           |
| ERROR_BUSYBOX_EPERM             | -11     | Operation is not permitted.                   |
| ERROR_BUSYBOX_EROFS             | -12     | The file is on a read-only filesystem.        |
| ERROR_BUSYBOX_EBADF             | -13     | Bad file descriptor.                          |
| ERROR_BUSYBOX_EINVAL            | -14     | Invalid argument.                             |
| ERROR_BUSYBOX_ESRCH             | -15     | No such process.                              |
| ERROR_BUSYBOX_ENOSPC            | -16     | No space left on device.                      |
| ERROR_BUSYBOX_ENOTEMPTY         | -17     | Directory is not empty.                       |
| ERROR_BUSYBOX_EWOULDBLOCK       | -18     | Operation would block.                        |
| ERROR_BUSYBOX_ETXTBSY           | -19     | Text file is busy.                            |
| ERROR_BUSYBOX_EOVERFLOW         | -20     | Value is too large to be stored in data type. |
| ERROR_BUSYBOX_EOPNOTSUPP        | -21     | Operation is not supported.                   |
| ERROR_BUSYBOX_ENXIO             | -22     | Device is not configured.                     |
| ERROR_BUSYBOX_ENODEV            | -23     | Operation is not supported by device.         |
| ERROR_BUSYBOX_ENFILE            | -24     | Too many open files are in system.            |
| ERROR_BUSYBOX_EDQUOT            | -25     | Disc quota is exceeded.                       |
| ERROR_BUSYBOX_EEXIST            | -26     | File exists.                                  |
| ERROR_BUSYBOX_EFBIG             | -27     | File is too large.                            |
| ERROR_BUSYBOX_EINTR             | -28     | Interrupted system call.                      |
| ERROR_BUSYBOX_EMFILE            | -29     | Too many open files.                          |
| ERROR_BUSYBOX_ENOSYS            | -30     | System does not support.                      |
| ERROR_BUSYBOX_INVALID_PARAM     | -31     | Invalid parameter.                            |
| ERROR_BUSYBOX_INSUFFICIENT_MEM  | -32     | Insufficient memory.                          |
| ERROR_BUSYBOX_CONNECT_HIBUS     | -33     | Can not connect to Hibus server.              |
| ERROR_BUSYBOX_WRONG_PROCEDURE   | -34     | Wrong procedure name.                         |
| ERROR_BUSYBOX_WRONG_JSON        | -35     | Wrong Json format.                            |
| ERROR_BUSYBOX_WORKING_DIRECTORY | -36     | Current working directory has not benn set.   |
| ERROR_BUSYBOX_UNKONOWN          | -37     | Unknow error.                                 |

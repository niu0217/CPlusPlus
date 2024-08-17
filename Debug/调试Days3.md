# 调试Days3

## 视频学习

【c++调试技术-哔哩哔哩】 https://b23.tv/TpRUPRx

代码地址： https://github.com/fuzhufang/ccdebug



## 使用gdb调试正在运行的进程

### 可能遇到的问题

Authentication is needed to run `/usr/bin/gdb' as the super user
Authenticating as: ubuntu

解决办法：https://blog.csdn.net/rainboatqin/article/details/124779096

简单点就是输入命令：

```bash
sudo echo 0| sudo tee /proc/sys/kernel/yama/ptrace_scope 
```



### launch.json

```json
{
  // Use IntelliSense to learn about possible attributes.
  // Hover to view descriptions of existing attributes.
  // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
  "version": "0.2.0",
  "configurations": [
    {
      "name": "(gdb) Attach",
      "type": "cppdbg",
      "request": "attach",
      "program": "${workspaceFolder}/CPP/tests/test",
      "processId": "${command:pickProcess}",
      "MIMode": "gdb",
      "setupCommands": [
          {
              "description": "Enable pretty-printing for gdb",
              "text": "-enable-pretty-printing",
              "ignoreFailures": true
          },
          {
              "description": "Set Disassembly Flavor to Intel",
              "text": "-gdb-set disassembly-flavor intel",
              "ignoreFailures": true
          }
      ]
    }
  ]
}
```




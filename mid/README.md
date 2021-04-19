# mid
实现客户端程序和服务器程序交互，支持的交互命令包括:
- pwd: 显示服务器应用程序启动时的当前目录
- cd: 改变服务器应用程序的当前路径
- ls: 显示服务器应用程序当前路径下的文件列表
- quit: 客户端进程退出, 但服务器端不能退出, 第二个客户可以再次连接上服务器

### 客户端程序:
使用`select`同时处理`stdin`和`socket`
- `select`发现标准输入事件，读出标准输入的字符，通过调用`write()`进行发送。
- `select`发现输入的是`quit`，则调用`shutdown()`关闭连接的写端。
- `select`发现套接字流有可读事件，则从`socket`中读出数据，并打印到`stdout`上；若读到了`EOF`，表示该客户端需要退出，直接退出循环，通过调用`exit()`退出。

运行效果
> // 第一次连接服务器
> $ ./mymclient 127.0.0.1 10087
>pwd
>/home/Code/network/mymhorse/build/bin
>cd ..
>pwd
>/home/Code/network/mymhorse/build
>cd ..
>pwd
>/home/Code/network/mymhorse
>ls
>build
>CMakeLists.txt
>lib
>mid
>README.md
>
>quit
>server closed connection

### 服务器端程序:
服务端程序需要两层循环，第一层控制多个客户连接，并未考虑并发，目前每次只能服务一个客户连接。第二层控制和单个连接的数据交互，因为可能会有多次命令交互过程。
- 第一层里通过`accept()`完成连接的建立，并获得**连接**socket。
- 第二层里通过调用`read()`从`socket`获取字节流。其中，buf缓冲每次使用前需调用`bzero()`初始化，以便复用。
1. 暂时没有考虑多个客户并发连接的情形，每次只能服务一个客户连接。
2. 把命令执行结果返回给已连接的客户端。
3. 服务器端不会因为客户端退出而直接退出。


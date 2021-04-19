# 如何编译与执行

在Linux环境下，先创建build目录，并到目录build下，执行cmake
```bash
mkdir build && cd build
cmake ../
```

再执行
```bash
make
```

所有的可执行程序在目录build/bin下面

# [mid](/mid/README.md)
实现客户端程序和服务器程序的简单交互
### 客户端程序:
使用`select`多路复用
- 从`stdin`接收字节流
- 通过`socket`读写，以及使用`shutdown`关闭半连接

### 服务器端程序:
使用`socket`进行读写，能够处理对端连接关闭，不过目前只能一次服务一个客户端连接，不具备并发服务的能力。




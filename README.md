# MyMrpc

仿照[mrpc](https://github.com/stolendance/mrpc)写的rpc框架，使用protobuf进行序列化与反序列化，使用muduo作为底层通信框架，实现了`Login`和`Hello`两个RPC函数。



## 依赖库安装

### Protobuf

本项目使用的protobuf版本是3.11.0，在安装之前一定要先卸载干净原来的protobuf，不然会产生版本冲突。点击[链接](https://pan.baidu.com/s/1IbVEDgnoLkF5vEzIJOnRuA?pwd=5rmm )下载压缩包，复制到ubuntu，执行以下命令

```bash
unzip protobuf-master.zip
cd protobuf-master
./autogen.sh
./configure
make
sudo make install
sudo ldconfig
```

### Muduo

运行以下命令

```bash
git clone https://github.com/chenshuo/muduo.git
./build.sh install
```

会在上一级目录中生成`build`文件夹，里面有一个`release-install-cpp11`文件夹，记住这个文件夹的路径

### Zookeeper

本项目使用的protobuf版本是3.4.14，点击[链接](https://pan.baidu.com/s/1wWggC05DQKDRBCKmX2Q1VQ?pwd=ubi3)下载压缩包，复制到ubuntu，执行以下命令

```bash
sudo tar -zxvf zookeeper-3.4.14.tar.gz -C /usr/local/
cd /usr/local/zookeeper-3.4.14
sudo cp conf/zoo_sample.cfg conf/zoo.cfg
sudo bin/zkServer.sh start
```

最后终端输出`Starting zookeeper ... STARTED`代表启动成功，关闭的命令是`sudo bin/zkServer.sh stop`

因为要用到zookeeper的库，所以还需要编译安装client库，执行以下命令会默认安装到`/usr/local/include/zookeeper`，这个路径也要记住

```bash
cd zookeeper-client/zookeeper-client-c
sudo ./configure
sudo make
sudo make install
```



## 使用方法

修改`MyMrpc/CMakeLists.txt`，将第11行、27行的两个路径换成你自己的，执行命令进行编译

```bash
mkdir build && cd build
cmake ..
make
```

进入`MyMrpc/bin`文件夹，可以看到生成了可执行文件`comsumer`和`provider`，执行`./provider -i test.conf`启动RPC服务（zookeeper要先启动）。开启一个新终端，执行`./comsumer -i test.conf`调用RPC服务，可以看到调用服务成功并正常结束。

![](https://raw.githubusercontent.com/InitialZJ/MarkdownPhotoes/main/res/provider.jpg)

![](https://raw.githubusercontent.com/InitialZJ/MarkdownPhotoes/main/res/comsumer.jpg)



## Tips

代码中的`user.pb.h`、`user.pb.cc`、`rpc_header.pb.h`和`rpc_header.pb.cc`都是使用protoc生成的，生成方法

```bash
# user
cd example
protoc user.proto --cpp_out="./"
```

```bash
# rpc_header
cd src
protoc rpc_header.proto --cpp_out="./"
mv rpc_header.pb.h ./include
```



## Todoes

1. 原作者实现了比较简陋的`MrpcLog`类，但并没有使用，后续可以考虑换成`MyMuduo`中的`Logger`类；
2. 客户端向服务器端发起请求的方式是原始的Linux库中的`connect`和`send`等方法，可以换成`muduo`中的`TcpClient`。
#include "RpcProvider.h"

#include <functional>
#include <iostream>

#include "MrpcApplication.h"
#include "Zookeeperutil.h"
#include "rpc_header.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service* service) {
  const google::protobuf::ServiceDescriptor* service_ptr =
      service->GetDescriptor();
  const std::string service_name = service_ptr->name();
  // 得到method的数量
  int n = service_ptr->method_count();
  std::map<std::string, const google::protobuf::MethodDescriptor*> method_dic;
  // 遍历方法，进行存储
  for (int i = 0; i < n; i++) {
    const google::protobuf::MethodDescriptor* method_ptr =
        service_ptr->method(i);
    const std::string method_name = method_ptr->name();
    method_dic[method_name] = method_ptr;
  }
  ServiceInfo sinfo;
  sinfo.service_ptr = service;
  sinfo.method_dic = method_dic;
  service_dic[service_name] = sinfo;

  for (auto& item : service_dic) {
    std::cout << "服务名：" << item.first << std::endl;
    for (auto& item2 : item.second.method_dic) {
      std::cout << "方法名：" << item2.first << std::endl;
    }
  }
}

// 启动rpc服务节点，开始提供rpc远程网络服务调用
void RpcProvider::Run() {
  MrpcConfig config_ins = MrpcApplication::GetInstance().getConfig();
  std::string ip = config_ins.Load("rpcserverip");
  uint16_t port = std::stoi(config_ins.Load("rpcserverport"));

  muduo::net::InetAddress address(ip, port);
  muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
  // 绑定连接回调和消息读写回调方法，分离网络代码和业务代码
  server.setConnectionCallback(
      std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
  server.setMessageCallback(
      std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
  server.setThreadNum(4);
  std::cout << "Rpc start at ip: " << ip << " port: " << port << std::endl;

  Zookeeperutil zk;
  zk.start();
  std::string ip_port = ip + ":" + config_ins.Load("rpcserverport");
  // 注册服务
  for (auto& server : service_dic) {
    std::string service_path = "/" + server.first;
    zk.create(service_path, "", 0);
    for (auto& func : server.second.method_dic) {
      std::string func_path = service_path + "/" + func.first;
      zk.create(func_path, ip_port, 0);
    }
  }
  // 启动网络服务
  server.start();
  m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {}

// 当有字节流时，会自动调用该函数
// 当函数解析字符流时，调用相应注册的函数
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp timestamp) {
  std::string buffer_str = buffer->retrieveAllAsString();
  // 截取前四个字节
  uint32_t len = 0;
  buffer_str.copy((char*)&len, 4, 0);
  // 解析rpc_header对象
  std::string rpc_header_str = buffer_str.substr(4, len);
  src::RpcHeader rpcheader = src::RpcHeader();
  if (!rpcheader.ParseFromString(rpc_header_str)) {
    std::cout << "rpcheader解析失败" << std::endl;
    return;
  }

  std::string service_name = rpcheader.service_name();
  std::string method_name = rpcheader.method_name();
  uint32_t args_len = rpcheader.args_len();

  // 找到对应的service method
  if (!service_dic.count(service_name)) {
    std::cout << "服务名不存在" << std::endl;
    return;
  }

  ServiceInfo service_info = service_dic[service_name];
  google::protobuf::Service* service = service_info.service_ptr;
  if (!service_info.method_dic.count(method_name)) {
    std::cout << "方法名不存在" << std::endl;
    return;
  }

  const google::protobuf::MethodDescriptor* method_des =
      service_info.method_dic[method_name];
  // 截取args，此时的args为request
  std::string args_str = buffer_str.substr(4 + len, args_len);
  // 解析args，需要获得service method下的request
  // 直接使用基类message即可，message是一个抽象类，不能实例化
  // 因此还需要找method的方法，创建一个新对象
  google::protobuf::Message* request =
      service->GetRequestPrototype(method_des).New();
  google::protobuf::Message* response =
      service->GetResponsePrototype(method_des).New();
  if (!request->ParseFromString(args_str)) {
    std::cout << "解析参数失败" << std::endl;
    return;
  }

  // 生成回调函数
  auto done = google::protobuf::NewCallback<RpcProvider,
                                            const muduo::net::TcpConnectionPtr&,
                                            google::protobuf::Message*>(
      this, &RpcProvider::callmeback, conn, response);

  // 调用函数
  service->CallMethod(method_des, nullptr, request, response, done);
}

void RpcProvider::callmeback(const muduo::net::TcpConnectionPtr& conn,
                             google::protobuf::Message* response) {
  // 序列化
  std::string response_str;
  if (!response->SerializeToString(&response_str)) {
    std::cout << "response序列化失败" << std::endl;
    return;
  }
  conn->send(response_str);
  conn->shutdown();
}

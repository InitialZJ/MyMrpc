#include "RpcProvider.h"

#include <functional>
#include <iostream>

#include "MrpcApplication.h"
#include "Zookeeperutil.h"
#include "rpc_header.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service* service) {
  const google::protobuf::ServiceDescriptor* service_ptr =
      service->GetDescroptor();
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
void RcpProvider::Run() {
	MrpcConfig config_ins = MrpcApplication::GetInstance().getConfig();
	std::string ip = config_ins.Load("rpcserverip");
	uint16_t port = std::stoi(config_ins.Load("rpcserverport"));

	muduo::net::InetAddress address(ip, port);
	muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
	server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
}

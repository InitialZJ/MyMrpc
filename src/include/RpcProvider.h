#ifndef RPC_PROVIDER_H_
#define RPC_PROVIDER_H_

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

#include <map>
#include <memory>
#include <string>

// rpc网络服务类
class RcpProvider {
 private:
  struct ServiceInfo {
    google::protobuf::Service* service_ptr;
    std::map<std::string, const google::protobuf::MethodDescriptor*> method_dic;
  };
  std::map<std::string, ServiceInfo> service_dic;
  muduo::net::EventLoop m_eventLoop;
};

#endif  // !RPC_PROVIDER_H_

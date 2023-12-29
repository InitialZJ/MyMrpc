#include <iostream>
#include <string>
#include <thread>

#include "MrpcApplication.h"
#include "MrpcLog.h"
#include "RpcProvider.h"
#include "user.pb.h"

std::string HelloQ(std::string name) {
  std::string s = "hello " + name + "!!!";
  return s;
}

class UserService : public fixbug::UserServiceRpc {
 public:
  void Hello(google::protobuf::RpcController* controller,
             const fixbug::HelloRequest* request,
             fixbug::HelloReponse* response, google::protobuf::Closure* done) {
    std::string name = request->name();
    std::string rs = HelloQ(name);
    std::cout << "doing local service: Hello, args: name: " << name << std::endl;
    response->set_hello(rs);
    done->Run();
  }

  bool Login(std::string name, std::string pwd) {
    std::cout << "doing local service: Login, args: name: " << name << ", pwd: " << pwd << std::endl;
    return true;
  }

  void Login(google::protobuf::RpcController* controller,
             const fixbug::LoginRequest* request,
             fixbug::LoginResponse* response, google::protobuf::Closure* done) {
    std::string name = request->name();
    std::string pwd = request->pwd();

    std::cout << "rpc_login name: " << name << std::endl;
    std::cout << "rpc_login pwd: " << pwd << std::endl;

		bool login_result = Login(name, pwd);
		fixbug::ResultCode* code = response->mutable_result();
    code->set_errcode(0);
    code->set_errmsg("");
    response->set_success(login_result);
    done->Run();
  }
};

int main(int argc, char** argv) {
  // 调用框架的初始化操作
  MrpcApplication::Init(argc, argv);
  // 把UserService对象发布到rpc节点上
  RpcProvider provider;
  provider.NotifyService(new UserService());
  // 启动rpc
  provider.Run();
  return 0;
}

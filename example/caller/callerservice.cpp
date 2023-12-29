#include <iostream>

#include "MrpcApplication.h"
#include "MrpcChannel.h"
#include "MrpcController.h"
#include "user.pb.h"

int main(int argc, char** argv) {
  MrpcApplication::Init(argc, argv);

  fixbug::UserServiceRpc_Stub stub(new MrpcChannel());

  fixbug::LoginRequest request;
  request.set_name("李志杰");
  request.set_pwd("123456");
  fixbug::LoginResponse response;
  MrpcController controller;

  stub.Login(&controller, &request, &response,
            nullptr);  // 会自动调用rpcchannel的callmethod

  if (!controller.Failed()) {
    // 得到执行后的response
    if (!response.success()) {
      std::cout << "远程rpc函数执行失败" << std::endl;
      std::cout << "errcode: " << response.result().errcode() << std::endl;
      std::cout << "errmsg: " << response.result().errmsg() << std::endl;
      return 0;
    }
    std::cout << "远程rpc函数执行成功" << std::endl;
    std::cout << "success: " << response.success() << std::endl;

    fixbug::HelloRequest request2;
    request2.set_name("李志杰");
    fixbug::HelloReponse response2;
    stub.Hello(nullptr, &request2, &response2, nullptr);

    std::cout << response2.hello() << std::endl;
    std::cout << "远程rpc函数执行成功" << std::endl;
  } else {
    std::cout << controller.ErrorText() << std::endl;
  }
  return 0;
}

#ifndef MRPC_CONTROLLER_H_
#define MRPC_CONTROLLER_H_

#include <google/protobuf/service.h>

#include <iostream>
#include <string>

class MrpcController : public google::protobuf::RpcController {
 public:
  void Reset();
  bool Failed() const;
  std::string ErrorText() const;
  void SetFailed(const std::string& reason);

  // 以下三个方法先不实现
  void StartCancel();
  bool IsCanceled() const;
  void NotifyOnCancel(google::protobuf::Closure* callback);

 private:
  bool isfail = false;
  std::string failtext;
};

#endif  // !MRPC_CONTROLLER_H_

#ifndef MRPC_APPLICATION_H_
#define MRPC_APPLICATION_H_

#include "MrpcConfig.h"

// 框架类，用单例模式进行设计
class MrpcApplication {
 public:
  static void Init(int argc, char** argv);
  static MrpcApplication& GetInstance();
  static MrpcConfig& getConfig();

 private:
  static MrpcConfig m_config;
  MrpcApplication(){};
  MrpcApplication(const MrpcApplication& other) = delete;
  MrpcApplication(MrpcApplication&& other) = delete;
  MrpcApplication& operator=(const MrpcApplication& other) = delete;
};

#endif  // !MRPC_APPLICATION_H_

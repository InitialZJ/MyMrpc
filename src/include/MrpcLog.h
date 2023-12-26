#ifndef MRPC_LOG_H_
#define MRPC_LOG_H_

#include <iostream>
#include <queue>
#include <string>

#include "Lockqueue.h"

class MrpcLog {
 public:
  static MrpcLog& getInstance();
  std::string read_log();
  void log(std::string data);

 private:
  Lockqueue<std::string> line;
  MrpcLog();
  MrpcLog(const MrpcLog&&) = delete;
  MrpcLog(const MrpcLog&) = delete;
  MrpcLog& operator=(const MrpcLog&) = delete;
};

#endif  // !MRPC_LOG_H_

#ifndef MRPC_CONFIG_H_
#define MRPC_CONFIG_H_

#include <string>
#include <unordered_map>

// 框架读取配置文件类
// rpcip
// rpcport
// zookeeper ip
// zookeeper port
class MrpcConfig {
 public:
  // 负责解析加载配置文件
  void LoadConfigFile(const char* config_file);
  std::string Load(const std::string& key);
  std::string Trim(std::string src_buf);

 private:
  std::unordered_map<std::string, std::string> m_configure;
};

#endif  // !MRPC_CONFIG_H_

#include "MrpcConfig.h"

#include <iostream>
#include <string>

void MrpcConfig::LoadConfigFile(const char* config_file) {
  FILE* pf = fopen(config_file, "r");
  if (pf == nullptr) {
    std::cout << config_file << " is invalid" << std::endl;
    exit(EXIT_FAILURE);
  }

  while (!feof(pf)) {
    // 三种情况：参考test.conf
    // 注释
    // 正确的配置项k=v
    // 去掉开头多余的空格
    char buf[512] = {0};
    fgets(buf, 512, pf);
    std::string src_buf(buf);
    // 去掉字符串前面的空格
    src_buf = Trim(src_buf);
    // 判断#的注释
    if (src_buf[0] == '#' || src_buf.empty()) {
      continue;
    }
    int idx = src_buf.find('=');
    if (idx == -1) {
      continue;
    }
    std::string key;
    std::string value;
    key = src_buf.substr(0, idx);
    src_buf = src_buf.substr(idx + 1, src_buf.size() - idx - 1);
    m_configure[key] = src_buf;
    std::cout << "port:" << src_buf << std::endl;
  }
}

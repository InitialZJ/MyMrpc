#ifndef ZOOKEEPERUTIL_H_
#define ZOOKEEPERUTIL_H_

#include <zookeeper.h>

#include <string>

class Zookeeperutil {
 public:
  Zookeeperutil();
  ~Zookeeperutil();
  void start();
  void create(std::string path, std::string data, int state);
  std::string getData(std::string path);

 private:
  zhandle_t* m_handle;
};

#endif  // !ZOOKEEPERUTIL_H_

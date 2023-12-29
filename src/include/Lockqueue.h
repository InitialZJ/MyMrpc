#ifndef LOCK_QUEUE_H_
#define LOCK_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Lockqueue {
 public:
  T pop();

  void push(T data);

 private:
  std::queue<T> line;
  std::mutex mt;
  std::condition_variable cond;
};

#endif  // !LOCK_QUEUE_H_

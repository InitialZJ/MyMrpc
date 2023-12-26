#ifndef LOCK_QUEUE_H_
#define LOCK_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Lockqueue {
 public:
  T pop() {
    std::unique_lock<std::mutex> lock(mt);
    while (line.size() == 0) {
      cond.wait(lock);
    }
    T data = line.front();
    line.pop();
    return data;
  }

  void push(T data) {
    std::lock_guard<std::mutex> lock(mt);
    line.push(data);
    cond.notify_one();
  }

 private:
  std::queue<T> line;
  std::mutex mt;
  std::condition_variable cond;
};

#endif  // !LOCK_QUEUE_H_

#include "Lockqueue.h"

template <typename T>
void Lockqueue<T>::push(T data) {
  std::lock_guard<std::mutex> lock(mt);
  line.push(data);
  cond.notify_one();
}

template <typename T>
T Lockqueue<T>::pop() {
  std::unique_lock<std::mutex> lock(mt);
  while (line.size() == 0) {
    cond.wait(lock);
  }
  T data = line.front();
  line.pop();
  return data;
}

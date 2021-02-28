#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<class T>
class BufferedChannel {
 public:
  explicit BufferedChannel(int size) : MaxSize(size) {}

  void Send(T value) {
    std::unique_lock<std::mutex> lock(Mutex);
    EmptyNotifier.wait(lock, [chan = this] {
      return (chan->Queue.size() < chan->MaxSize) || chan->isClosed;
    });
    if (isClosed || Queue.size() == MaxSize) {
      throw std::runtime_error("Channel was closed.");
    }
    Queue.push(std::move(value));
    FullNotifier.notify_all();
  }

  std::optional<T> Recv() {
    std::unique_lock<std::mutex> lock(Mutex);
    FullNotifier.wait(lock, [chan = this] {
      return !(chan->Queue.empty()) || chan->isClosed;
    });
    if (isClosed && Queue.empty()) {
      return std::nullopt;
    }
    T result(std::move(Queue.front()));
    Queue.pop();
    EmptyNotifier.notify_all();
    return result;
  }

  void Close() {
    std::unique_lock<std::mutex> lock(Mutex);
    isClosed = true;
    FullNotifier.notify_all();
    EmptyNotifier.notify_all();
  }

 private:
  int MaxSize;
  std::queue<T> Queue{};
  std::mutex Mutex{};
  std::condition_variable FullNotifier{};
  std::condition_variable EmptyNotifier{};
  bool isClosed = false;
};

#endif  // BUFFERED_CHANNEL_H_

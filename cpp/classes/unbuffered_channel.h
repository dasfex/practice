#ifndef UNBUFFERED_CHANNEL_H_
#define UNBUFFERED_CHANNEL_H_

#include <condition_variable>
#include <mutex>
#include <optional>

template<class T>
class UnbufferedChannel {
 public:
  void Send(T value) {
    std::lock_guard<std::mutex> lock(BlockSendMutex);
    std::unique_lock<std::mutex> guard(CommonMutex);

    SendNotifier.wait(guard, [this] {
      return !isSended && (ReceiversCount > 0 || isClosed);
    });
    if (isClosed) {
      throw std::runtime_error("Channel is closed.");
    }
    isSended = true;
    Value = std::move(value);
    RecvNotifier.notify_one();
  }

  std::optional<T> Recv() {
    std::lock_guard<std::mutex> lock(BlockRecvMutex);
    std::unique_lock<std::mutex> guard(CommonMutex);

    ++ReceiversCount;
    SendNotifier.notify_one();
    RecvNotifier.wait(guard, [this] {
      return isSended || isClosed;
    });
    if (!isSended) {
      return std::nullopt;
    }
    T result(std::move(Value));
    --ReceiversCount;
    isSended = false;
    SendNotifier.notify_one();
    return result;
  }

  void Close() {
    std::unique_lock<std::mutex> lock(CommonMutex);
    isClosed = true;
    SendNotifier.notify_all();
    RecvNotifier.notify_all();
  }

 private:
  T Value = T();
  bool isSended = false;
  bool isClosed = false;
  int ReceiversCount = 0;
  std::mutex BlockSendMutex{};
  std::mutex BlockRecvMutex{};
  std::mutex CommonMutex{};
  std::condition_variable SendNotifier{};
  std::condition_variable RecvNotifier{};
};

#endif  // UNBUFFERED_CHANNEL_H_

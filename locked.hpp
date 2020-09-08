#include <atomic>             // for atomic
#include <condition_variable> // for condition_variable
#include <memory>             // for shared_ptr
#include <mutex>              // for mutex, lock_guard
#include <stddef.h>           // for size_t
#include <utility>            // for forward

template <typename T> class locked {
  using Type = T;

  Type inner;

  mutable std::mutex exclusiveAccess;
  mutable std::atomic<size_t> viewers{0};
  mutable std::condition_variable noMoreViewersConditionVariable;
  mutable std::mutex noMoreViewersMutex;
public:
  template <typename... Args>
  locked(Args... args) : inner(std::forward<Args>(args)...) {}


  template <typename F, typename... Args>
  decltype(auto) view(F f, Args... args) const {
    {
      std::unique_lock<std::mutex> _(exclusiveAccess);
      viewers++;
    }
    std::shared_ptr<void> _(nullptr, [&](...) {
      if (--viewers == 0) {
        std::unique_lock<std::mutex> noMoreViewersLock(noMoreViewersMutex);
        noMoreViewersConditionVariable.notify_one();
      }
    });
    return f(inner, std::forward<Args>(args)...);
  }

  template <typename F, typename... Args>
  decltype(auto) mut(F f, Args... args) {
    std::unique_lock<std::mutex> _(exclusiveAccess);
    std::unique_lock<std::mutex> noMoreViewersLock(noMoreViewersMutex);
    noMoreViewersConditionVariable.wait(noMoreViewersLock,
                                        [&]() { return viewers == 0; });
    return f(inner, std::forward<Args>(args)...);
  }

  Type get() const {
      return view([](const Type& t){return Type(t);});
  }
};

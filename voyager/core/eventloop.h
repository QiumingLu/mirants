#ifndef VOYAGER_CORE_EVENTLOOP_H_
#define VOYAGER_CORE_EVENTLOOP_H_

#include <unistd.h>

#include <functional>
#include <memory>
#include <vector>

#include "voyager/core/timerlist.h"
#include "voyager/port/currentthread.h"
#include "voyager/port/mutexlock.h"

namespace voyager {

class Dispatch;
class EventPoller;

class EventLoop {
 public:
  typedef std::function<void()> Func;

  EventLoop();
  ~EventLoop();

  void Loop();

  void RunInLoop(const Func& func);
  void QueueInLoop(const Func& func);

  void RunInLoop(Func&& func);
  void QueueInLoop(Func&& func);

  TimerList::Timer* RunAt(uint64_t micros_value,
                          const TimerProcCallback& cb);
  TimerList::Timer* RunAfter(uint64_t micros_delay,
                             const TimerProcCallback& cb);
  TimerList::Timer* RunEvery(uint64_t micros_interval,
                             const TimerProcCallback& cb);

  TimerList::Timer* RunAt(uint64_t micros_value, TimerProcCallback&& cb);
  TimerList::Timer* RunAfter(uint64_t micros_delay, TimerProcCallback&& cb);
  TimerList::Timer* RunEvery(uint64_t micros_interval, TimerProcCallback&& cb);

  void RemoveTimer(TimerList::Timer* t);

  void AssertInMyLoop() {
    if (!IsInMyLoop()) {
      Abort();
    }
  }

  bool IsInMyLoop() const { return tid_ == port::CurrentThread::Tid(); }

  void Exit();

  // the eventloop of current thread.
  static EventLoop* RunLoop();

  // only internal use
  void RemoveDispatch(Dispatch* dispatch);
  void UpdateDispatch(Dispatch* dispatch);
  bool HasDispatch(Dispatch* dispatch);

 private:
  void RunFuncs();
  void HandleRead();
  void Abort();
  void WakeUp();

  bool exit_;
  bool run_;

  const uint64_t tid_;
  std::unique_ptr<EventPoller> poller_;
  std::unique_ptr<TimerList> timers_;

  #ifdef __linux__
  int wakeup_fd_;
  #else
  int wakeup_fd_[2];
  #endif

  std::unique_ptr<Dispatch> wakeup_dispatch_;

  port::Mutex mu_;
  std::vector<Func> funcs_;

  // No copying allowed
  EventLoop(const EventLoop&);
  void operator=(const EventLoop&);
};

}  // namespace voyager

#endif  // VOYAGER_CORE_EVENTLOOP_H_

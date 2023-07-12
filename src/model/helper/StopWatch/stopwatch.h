#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_STOPWATCH_STOPWATCH_H
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_STOPWATCH_STOPWATCH_H

#include <chrono>
#include <type_traits>

namespace ng {
const bool steady_clock = true;
const bool system_clock = false;

// ----------- StopWatch class -----------
// Main information:
// 1) The class is wrap of std::chrono class to easier use
template <bool IsSteadyClock>
class StopWatch {
 public:
  using time_point =
      std::conditional_t<IsSteadyClock, std::chrono::steady_clock::time_point,
                         std::chrono::system_clock::time_point>;
  using clock = std::conditional_t<IsSteadyClock, std::chrono::steady_clock,
                                   std::chrono::system_clock>;

 private:
  time_point begin_;
  time_point end_;

  struct CurrentTime {
    long long nanoseconds = 0;
    double microseconds = 0;
    double milliseconds = 0;
    double seconds = 0.0;
  };

  CurrentTime time_;

 public:
  StopWatch() = default;

  void start() {
    reset();
    begin_ = clock::now();
  }

  void stop() {
    end_ = clock::now();
    countcurrenttime();
  }

  long long nanoseconds() { return time_.nanoseconds; }
  double microseconds() { return time_.microseconds; }
  double milliseconds() { return time_.milliseconds; }
  double seconds() { return time_.seconds; }

 private:
  void reset() {
    time_.nanoseconds = 0;
    time_.microseconds = 0.0;
    time_.milliseconds = 0.0;
    time_.seconds = 0.0;
  }
  void countcurrenttime() {
    time_.nanoseconds =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - begin_)
            .count();
    time_.microseconds = time_.nanoseconds / 1000.0;
    time_.milliseconds = time_.nanoseconds / 1000000.0;
    time_.seconds = time_.nanoseconds / 1000000000.0;
  }
};
}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_STOPWATCH_STOPWATCH_H

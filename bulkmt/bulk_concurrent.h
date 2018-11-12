#pragma once

#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "bulk.h"
#include "stats.h"

namespace hw10 {

template<size_t ThreadCount>
class BulkConcurrentObserver : public hw7::BulkObserver
{
public:
  BulkConcurrentObserver(std::array<std::string, ThreadCount> names)
  {
    for (size_t i = 0; i < ThreadCount; ++i) {
      m_threads[i] = std::thread{&BulkConcurrentObserver::worker, this, i};
      m_stats[i] = Stats{names[i]};
      m_stats[i].resetMetrics({"bulks", "commands"});
    }
  }

  ~BulkConcurrentObserver()
  {
    stop();
  }

  void stop() final override
  {
    m_stop = true;
    m_ready.notify_all();

    for (auto& t : m_threads)
      if (t.joinable())
        t.join();
  }

  void update(const hw7::BulkTime& time, const hw7::Bulk& bulk) final override
  {
    std::lock_guard<std::mutex> lock{m_queueMutex};
    m_queue.emplace(time, bulk);
    m_ready.notify_all();
  }

  auto stats() const
  {
    return m_stats;
  }

private:
  virtual void handle(const hw7::BulkTime&, const hw7::Bulk&) = 0;

  void worker(int statIndex)
  {
    while (true) {
      hw7::BulkTime time;
      hw7::Bulk bulk;

      {
        std::unique_lock<std::mutex> lock{m_queueMutex};
        m_ready.wait(lock, [this]() { return !m_queue.empty() || m_stop; });
        if (m_stop && m_queue.empty())
          return;

        std::tie(time, bulk) = m_queue.front();
        m_queue.pop();
      }

      handle(time, bulk);

      m_stats[statIndex].takeCountOf("bulks", 1);
      m_stats[statIndex].takeCountOf("commands", bulk.size());
    }
  }

  std::array<std::thread, ThreadCount> m_threads;
  std::queue<std::tuple<hw7::BulkTime, hw7::Bulk>> m_queue;
  std::mutex m_queueMutex;
  std::atomic<bool> m_stop{false};
  std::condition_variable m_ready;
  std::array<Stats, ThreadCount> m_stats;
};

} // hw10

#include <cassert>

#include "stats.h"

hw10::Stats::Stats(const std::string& name)
  : m_name{name}
  , m_metrics{}
{
}

void hw10::Stats::resetMetrics(const std::vector<std::string>& metrics)
{
  m_metrics.clear();
  for (auto i : metrics)
    m_metrics[i] = 0;
}

void hw10::Stats::takeCountOf(const std::string& metric, size_t count)
{
  auto i = m_metrics.find(metric);
  assert(i != m_metrics.end());

  i->second += count;
}

std::string hw10::Stats::name() const
{
  return m_name;
}

auto hw10::Stats::begin() const
{
  return m_metrics.begin();
}

auto hw10::Stats::end() const
{
  return m_metrics.end();
}

std::ostream& operator<<(std::ostream& os, const hw10::Stats& stats)
{
  os << stats.name() << ":\n";
  for (auto s : stats)
    os << "  " << s.first << ": " << s.second << "\n";
  os << std::endl;

  return os;
}

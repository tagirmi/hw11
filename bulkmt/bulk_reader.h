#pragma once

#include <list>
#include <memory>

#include "bulk.h"
#include "stats.h"

namespace hw7 {

namespace details {

class BulkCollector;

} // details

} // hw7

namespace hw10 {

class BulkReader
{
public:
  explicit BulkReader(size_t bulkSize);
  ~BulkReader();

  void subscribe(const std::shared_ptr<hw7::BulkObserver>&);
  void read(std::istream&);

  Stats stats() const;

private:
  void notify(const hw7::BulkTime&, const hw7::Bulk&);
  void stop();

  std::list<std::weak_ptr<hw7::BulkObserver>> m_observers;
  std::unique_ptr<hw7::details::BulkCollector> m_bulkCollector;

  Stats m_stats;
};

} // hw10

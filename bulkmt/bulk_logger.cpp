#include <fstream>
#include <sstream>
#include <atomic>

#include "bulk_logger.h"

namespace {

std::string getBulkLogName(const hw7::BulkTime& time)
{
  static std::atomic<int> id{0};

  std::stringstream ss;
  ss << "bulk" << time.time_since_epoch().count() << "_" << std::atomic_fetch_add(&id, 1) << ".log";
  return ss.str();
}

} // namespace

hw10::BulkLogger::BulkLogger()
  : BulkConcurrentObserver{{"file1", "file2"}}
{
}

void hw10::BulkLogger::handle(const hw7::BulkTime& time, const hw7::Bulk& bulk)
{
  std::ofstream fs{getBulkLogName(time)};

  fs << bulk;

  fs.close();
}

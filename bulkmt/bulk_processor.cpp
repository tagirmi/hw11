#include <iostream>

#include "bulk_processor.h"

hw10::BulkProcessor::BulkProcessor()
  : BulkConcurrentObserver{{"log"}}
{
}

void hw10::BulkProcessor::handle(const hw7::BulkTime&, const hw7::Bulk& bulk)
{
  std::cout << bulk << std::endl;
}

#pragma once

#include "bulk_concurrent.h"

namespace hw10 {

class BulkLogger final : public BulkConcurrentObserver<2>
{
public:
  BulkLogger();
  ~BulkLogger() = default;

private:
  void handle(const hw7::BulkTime&, const hw7::Bulk&) override;
};

} // hw10

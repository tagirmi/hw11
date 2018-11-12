#pragma once

#include "bulk_concurrent.h"

namespace hw10 {

class BulkProcessor final : public BulkConcurrentObserver<1>
{
public:
  BulkProcessor();
  ~BulkProcessor() = default;

private:
  void handle(const hw7::BulkTime&, const hw7::Bulk&) override;
};

} // hw7

#pragma once

#include <memory>
#include <string>
#include <functional>

#include "bulk.h"

namespace hw7 {

namespace details {

class BulkState;

class BulkCollector
{
public:
  using ProcessBulk = std::function<void(const BulkTime&, const Bulk&)>;

  BulkCollector(size_t bulkSize, ProcessBulk);
  ~BulkCollector() = default;

  void add(const std::string&);
  void endData();

  void setState(std::unique_ptr<BulkState>);

  void addToBulk(const std::string&);
  bool isBulkPrepared() const;
  void processBulk();
  void createBulk();

private:
  std::unique_ptr<BulkState> m_state;

  size_t m_bulkSize;
  ProcessBulk m_processBulk;
  Bulk m_bulk;
  BulkTime m_bulkTime;
};

class BulkState
{
public:
  virtual ~BulkState() = default;

  virtual void onEnter(BulkCollector&) = 0;
  virtual void add(BulkCollector&, const std::string&) = 0;
  virtual void endData(BulkCollector&) = 0;
};

class EmptyBulk : public BulkState
{
public:
  void onEnter(BulkCollector&) override;
  void add(BulkCollector&, const std::string&) override;
  void endData(BulkCollector&) override;
};

class DynamicBulk : public BulkState
{
public:
  void onEnter(BulkCollector&) override;
  void add(BulkCollector&, const std::string&) override;
  void endData(BulkCollector&) override;

private:
  size_t m_nestedBracketCount{0};
};

class FixedBulk : public BulkState
{
public:
  void onEnter(BulkCollector&) override;
  void add(BulkCollector&, const std::string&) override;
  void endData(BulkCollector&) override;
};

class PreparedBulk : public BulkState
{
public:
  void onEnter(BulkCollector&) override;
  void add(BulkCollector&, const std::string&) override;
  void endData(BulkCollector&) override;
};

} // details

} // hw7

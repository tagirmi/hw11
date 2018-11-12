#include "bulk_collector.h"

namespace {

const char* openBracket = "{";
const char* closeBracket = "}";

}

hw7::details::BulkCollector::BulkCollector(size_t bulkSize, ProcessBulk processBulk)
  : m_state{std::make_unique<EmptyBulk>()}
  , m_processBulk{std::move(processBulk)}
  , m_bulkSize{bulkSize}
  , m_bulk{}
  , m_bulkTime{}
{
}

void hw7::details::BulkCollector::add(const std::string& cmd)
{
  m_state->add(*this, cmd);
}

void hw7::details::BulkCollector::endData()
{
  m_state->endData(*this);
}

void hw7::details::BulkCollector::setState(std::unique_ptr<BulkState> state)
{
  m_state = std::move(state);
  m_state->onEnter(*this);
}

void hw7::details::BulkCollector::addToBulk(const std::string& cmd)
{
  m_bulk.emplace_back(cmd);
}

bool hw7::details::BulkCollector::isBulkPrepared() const
{
  return !(m_bulk.size() < m_bulkSize);
}

void hw7::details::BulkCollector::processBulk()
{
  m_processBulk(m_bulkTime, m_bulk);
}

void hw7::details::BulkCollector::createBulk()
{
  m_bulkTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
  m_bulk.clear();
}

void hw7::details::EmptyBulk::onEnter(BulkCollector& collector)
{
}

void hw7::details::EmptyBulk::add(BulkCollector& collector, const std::string& cmd)
{
  if (cmd == openBracket) {
    collector.setState(std::make_unique<DynamicBulk>());
    return;
  }

  collector.createBulk();
  collector.addToBulk(cmd);

  if (collector.isBulkPrepared())
    collector.setState(std::make_unique<PreparedBulk>());
  else
    collector.setState(std::make_unique<FixedBulk>());
}

void hw7::details::EmptyBulk::endData(BulkCollector&)
{
}

void hw7::details::DynamicBulk::onEnter(BulkCollector& collector)
{
  collector.createBulk();
}

void hw7::details::DynamicBulk::add(BulkCollector& collector, const std::string& cmd)
{
  if (cmd == openBracket) {
    ++m_nestedBracketCount;
  }
  else if (cmd == closeBracket) {
    if (m_nestedBracketCount > 0)
      --m_nestedBracketCount;
    else
      collector.setState(std::make_unique<PreparedBulk>());
  }
  else {
    collector.addToBulk(cmd);
  }
}

void hw7::details::DynamicBulk::endData(BulkCollector&)
{
}

void hw7::details::FixedBulk::onEnter(BulkCollector&)
{
}

void hw7::details::FixedBulk::add(BulkCollector& collector, const std::string& cmd)
{
  if (cmd == openBracket) {
    collector.processBulk();
    collector.setState(std::make_unique<DynamicBulk>());
    return;
  }

  collector.addToBulk(cmd);

  if (collector.isBulkPrepared())
    collector.setState(std::make_unique<PreparedBulk>());
}

void hw7::details::FixedBulk::endData(BulkCollector& collector)
{
  collector.processBulk();
}

void hw7::details::PreparedBulk::onEnter(BulkCollector& collector)
{
  collector.processBulk();
  collector.setState(std::make_unique<EmptyBulk>());
}

void hw7::details::PreparedBulk::add(BulkCollector&, const std::string&)
{
}

void hw7::details::PreparedBulk::endData(BulkCollector&)
{
}

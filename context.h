#pragma once

#include "bulkmt/bulk_reader.h"
#include "bulkmt/bulk_processor.h"
#include "bulkmt/bulk_logger.h"

namespace hw11 {

class Context
{
public:
  explicit Context(size_t bulkSize);
  ~Context() = default;

  void recieve(const char* data, size_t size);

private:
  hw10::BulkReader m_reader;
  std::shared_ptr<hw10::BulkLogger> m_logger;
  std::shared_ptr<hw10::BulkProcessor> m_processor;
};

class ContextManager
{
public:
  using handle_t = void*;

  ContextManager() = default;
  ~ContextManager() = default;
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;

  static ContextManager& instance()
  {
    static ContextManager pool;
    return pool;
  }

  handle_t createContext(size_t bulk);
  void destroyContext(handle_t handle);
  hw11::Context* findContext(handle_t handle);

private:
  std::list<std::unique_ptr<hw11::Context>> m_storage;
};

} // hw11

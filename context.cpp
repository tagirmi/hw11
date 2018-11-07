#include <algorithm>

#include "context.h"

hw11::Context::Context(size_t bulkSize)
  : m_reader{bulkSize}
  , m_stream{}
  , m_thread{&Context::worker, this}
{
  m_processor = std::make_shared<hw10::BulkProcessor>();
  m_logger = std::make_shared<hw10::BulkLogger>();

  m_reader.subscribe(m_processor);
  m_reader.subscribe(m_logger);
}

hw11::Context::~Context()
{
  m_thread.join();
}

void hw11::Context::recieve(const char* data, size_t size)
{
  m_stream.write(data, size);
}

void hw11::Context::worker()
{
  m_reader.read(m_stream);
}

hw11::ContextManager::handle_t hw11::ContextManager::createContext(size_t bulk)
{
  auto context = std::make_unique<Context>(bulk);
  handle_t handle = context.get();
  m_storage.emplace_back(std::move(context));
  return handle;
}

void hw11::ContextManager::destroyContext(handle_t handle)
{
  m_storage.remove_if([handle](auto& context){
    return context.get() == handle;
  });
}

hw11::Context* hw11::ContextManager::findContext(handle_t handle)
{
  auto context = std::find_if(m_storage.begin(), m_storage.end(),
                              [handle](auto& context){
    return context.get() == handle;
  });
  if (context == m_storage.end())
    return nullptr;

  return context->get();
}

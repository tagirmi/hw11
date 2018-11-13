#include <algorithm>

#include "context.h"

namespace {

std::vector<std::string> split(std::string& str, char d = '\n')
{
  std::vector<std::string> r;
  r.reserve(std::count(str.cbegin(), str.cend(), d) + 1);

  std::string::size_type stop = str.find_first_of(d);
  while(stop != std::string::npos) {
    r.emplace_back(str.substr(0, stop));
    str.erase(0, stop + 1);

    stop = str.find_first_of(d);
  }

  return r;
}

} // namespace

hw11::Context::Context(size_t bulkSize)
  : m_reader{bulkSize}
  , m_thread{&Context::worker, this}
{
  m_processor = std::make_shared<hw10::BulkProcessor>();
  m_logger = std::make_shared<hw10::BulkLogger>();

  m_reader.subscribe(m_processor);
  m_reader.subscribe(m_logger);
}

hw11::Context::~Context()
{
  m_stop = true;
  m_ready.notify_one();

  m_thread.join();
}

void hw11::Context::recieve(const char* data, size_t size)
{
  std::vector<std::string> lines;

  {
    std::lock_guard<std::mutex> lock(m_currentDataMutex);
    m_currentData += std::string{data, size};
    lines = split(m_currentData);
  }

  {
    std::lock_guard<std::mutex> lock(m_linesMutex);
    m_lines.insert(m_lines.cend(), lines.cbegin(), lines.cend());
  }
  m_ready.notify_one();
}

void hw11::Context::worker()
{
  while (true) {
    std::vector<std::string> lines;

    {
      std::unique_lock<std::mutex> lock(m_linesMutex);
      m_ready.wait(lock, [this]() { return !m_lines.empty() || m_stop; });

      if (m_stop && m_lines.empty())
        break;

      lines = m_lines;
      m_lines.clear();
    }

    for (const auto& i : lines)
      m_reader.addLine(i);
  }

  m_reader.endLine();
}

hw11::ContextManager::handle_t hw11::ContextManager::createContext(size_t bulk)
{
  auto context = std::make_unique<Context>(bulk);
  handle_t handle = context.get();

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_storage.emplace_back(std::move(context));
  }

  return handle;
}

void hw11::ContextManager::destroyContext(handle_t handle)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_storage.remove_if([handle](auto& context) {
    return context.get() == handle;
  });
}

hw11::Context* hw11::ContextManager::findContext(handle_t handle)
{
  Context* context = nullptr;

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = std::find_if(m_storage.begin(), m_storage.end(), [handle](auto& context){
      return context.get() == handle;
    });

    if (iter != m_storage.end())
      context = iter->get();
  }

  return context;
}

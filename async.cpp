#include <list>
#include <memory>

#include "async.h"
#include "context.h"

namespace async {

handle_t connect(std::size_t bulk) {
  return hw11::ContextManager::instance().createContext(bulk);
}

void receive(handle_t handle, const char* data, std::size_t size)
{
  auto context = hw11::ContextManager::instance().findContext(handle);
  if (context)
    context->recieve(data, size);
}

void disconnect(handle_t handle)
{
  hw11::ContextManager::instance().destroyContext(handle);
}

}

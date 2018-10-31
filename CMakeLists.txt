cmake_minimum_required(VERSION 3.9)
project(async)

set(CMAKE_CXX_STANDARD 14)

add_library(async SHARED async.cpp async.h context.cpp context.h)

add_library(bulkmt bulkmt/bulk_reader.cpp bulkmt/bulk_logger.cpp bulkmt/bulk_processor.cpp
  bulkmt/bulk_collector.cpp bulkmt/stats.cpp)

add_executable(async_cli main.cpp)

target_link_libraries(async bulkmt)
target_link_libraries(async_cli async)

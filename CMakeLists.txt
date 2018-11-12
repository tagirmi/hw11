cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
  set(BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
  set(BUILD 0)
endif()

project(async VERSION 1.0.${BUILD})

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} SHARED async.cpp async.h context.cpp context.h)

file(GLOB BULKMT_SRC bulkmt/*.cpp bulkmt/*.h)

add_library(bulkmt ${BULKMT_SRC})

add_executable(async_cli main.cpp)

set_target_properties(${PROJECT_NAME} bulkmt async_cli PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(${PROJECT_NAME} bulkmt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(async_cli ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES async.h DESTINATION include)

cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
  set(BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
  set(BUILD 0)
endif()

project(async VERSION 1.0.${BUILD})

find_package(Threads REQUIRED)

add_library(async SHARED async.cpp async.h context.cpp context.h)
set_property(TARGET async PROPERTY POSITION_INDEPENDENT_CODE ON)

file(GLOB BULKMT_SRC bulkmt/*.cpp bulkmt/*.h)

add_library(bulkmt ${BULKMT_SRC})
set_property(TARGET bulkmt PROPERTY POSITION_INDEPENDENT_CODE ON)

add_executable(async_cli main.cpp)

set_target_properties(async bulkmt async_cli PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(async bulkmt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(async_cli async)

install(TARGETS async DESTINATION lib)
install(FILES async.h DESTINATION include)
install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT tagirmi@gmail.com)

include (CPack)

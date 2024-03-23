cmake_minimum_required(VERSION 3.13)

enable_language(C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(WITH_LOCKS "add lock support if available" ON)
option(BUILD_TESTS "enable testing" ON)
option(SHARED "build shared library(off for static library)" ON)

if (WITH_LOCKS AND NOT WIN32)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
endif()

project(anomap VERSION 1.0.0 LANGUAGES C)
if (SHARED)
  add_library(${PROJECT_NAME} SHARED)
else()
  add_library(${PROJECT_NAME} STATIC)
endif()
target_sources(${PROJECT_NAME} PRIVATE
  anomap.c
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if (NOT WITH_LOCKS)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ANOMAP_NATIVE_LOCKS=0)
else()
  if (CMAKE_USE_PTHREADS_INIT)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ANOMAP_NATIVE_LOCKS=1)
  endif()
endif()

if(NOT WIN32)
  target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -pipe)

  include(GNUInstallDirs)
  install(DIRECTORY include/ DESTINATION include)
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
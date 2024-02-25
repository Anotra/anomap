cmake_minimum_required(VERSION 3.13)

enable_language(C)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

option(WITH_LOCKS "add lock support if available" ON)

project(anomap VERSION 1.0.0 LANGUAGES C)
add_library(${PROJECT_NAME} SHARED anomap.c)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
if (NOT WITH_LOCKS)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ANOMAP_NATIVE_LOCKS=0)
else()
  if (CMAKE_USE_PTHREADS_INIT)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ANOMAP_NATIVE_LOCKS=1)
  endif()
endif()

if(NOT WIN32)
  target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -pipe)

  include(GNUInstallDirs)
  install(FILES anomap.h DESTINATION include/anotra)
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

project(anotest)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} anomap)

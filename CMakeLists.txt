cmake_minimum_required(VERSION 3.13)

enable_language(C)

project(anomap VERSION 1.0.0 LANGUAGES C)
add_library(${PROJECT_NAME} SHARED anomap.c)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

if(NOT WIN32)
  target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -pipe)
  
  include(GNUInstallDirs)
  install(FILES anomap.h DESTINATION include/ano)
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

project(anotest)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} anomap)

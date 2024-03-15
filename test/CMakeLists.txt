project(anotest)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE anomap)

if (WITH_LOCKS)
  if (CMAKE_USE_PTHREADS_INIT)
    target_sources(${PROJECT_NAME} PRIVATE locks-pthread.c)
  endif()
  target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_LOCKS)
  add_test(locks ${PROJECT_NAME} locks)
endif()

target_sources(${PROJECT_NAME} PRIVATE ordering.c)
add_test(ordering ${PROJECT_NAME} ordering)
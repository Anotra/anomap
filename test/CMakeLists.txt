project(anotest)
add_executable(${PROJECT_NAME} main.c
  locks-pthread.c
  ordering.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE anomap)


if (WITH_LOCKS)
  add_test(locks ${PROJECT_NAME} locks)
endif()

add_test(ordering ${PROJECT_NAME} ordering)
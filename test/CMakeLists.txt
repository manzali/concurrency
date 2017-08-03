include_directories(
  ${CONCURRENCY_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

add_executable(
  t_scheduler
  t_scheduler.cpp
)

target_link_libraries(
  t_scheduler
  ${Boost_LIBRARIES}
)

add_executable(
  t_spin_queue
  t_spin_queue.cpp
)

target_link_libraries(
  t_spin_queue
  ${Boost_LIBRARIES}
)

add_executable(
  t_queue
  t_queue.cpp
)

add_executable(sender_find_consistent_test src/sender_find_test.cpp)
target_link_libraries(sender_find_consistent_test sender_find_consistent utils GTest::gtest)

add_test(NAME              sender_find_consistent_test 
         COMMAND           sender_find_consistent_test
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data_consistent)


add_executable(sender_find_parallel_test src/sender_find_test.cpp)
target_link_libraries(sender_find_parallel_test sender_find_parallel utils GTest::gtest)

add_test(NAME              sender_find_parallel_test 
         COMMAND           sender_find_parallel_test
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data_parallel)


add_executable(compare_reports src/compare_reports.cpp)
target_link_libraries(compare_reports mails GTest::gtest)

add_test(NAME              compare_reports
         COMMAND           compare_reports
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


if(VALGRIND OR THREAD_SANITIZER)
    target_compile_definitions(sender_find_consistent_test PRIVATE TIME_ITERS=1 STRESS_ITERS=1 USERS_NUM=1000 MAILS_NUM=1000)
    target_compile_definitions(sender_find_parallel_test PRIVATE TIME_ITERS=1 STRESS_ITERS=1 USERS_NUM=1000 MAILS_NUM=1000)
endif()

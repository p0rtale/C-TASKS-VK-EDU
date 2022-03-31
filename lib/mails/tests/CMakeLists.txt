add_executable(mails_test src/mails_test.cpp)
target_link_libraries(mails_test mails GTest::gtest)

add_test(NAME    mails_test 
         COMMAND mails_test)

add_executable(generators_test src/generators_test.cpp)
target_link_libraries(generators_test mails GTest::gtest)

add_test(NAME    generators_test 
         COMMAND generators_test)

add_executable(utils_test src/utils_test.cpp)
target_link_libraries(utils_test utils GTest::gtest)

add_test(NAME    utils_test 
         COMMAND utils_test)

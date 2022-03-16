add_executable(contract_test src/contract_test.cpp)
target_link_libraries(contract_test contract GTest::gtest)

add_test(NAME    contract_test 
         COMMAND contract_test)

add_library(contract STATIC src/contract.c)
target_include_directories(contract PUBLIC include)
target_link_libraries(contract PUBLIC errors utils)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

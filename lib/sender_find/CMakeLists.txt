add_library(sender_find_consistent STATIC src/sender_find_consistent.c)
target_include_directories(sender_find_consistent PUBLIC include)
target_link_libraries(sender_find_consistent PUBLIC errors mails)

add_library(sender_find_parallel SHARED src/sender_find_parallel.c)
target_include_directories(sender_find_parallel PUBLIC include)
target_link_libraries(sender_find_parallel PUBLIC errors mails)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

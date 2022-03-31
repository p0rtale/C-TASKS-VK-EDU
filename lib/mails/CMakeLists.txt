add_library(mails STATIC src/mails.c src/generators.c)
target_include_directories(mails PUBLIC include)
target_link_libraries(mails PUBLIC errors utils)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

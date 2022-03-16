cmake_minimum_required(VERSION 3.20)

project(hw1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_compile_options(-O0 -g -Wall -Werror -Wpedantic -Wextra)

option(ADRESS_SANITIZER "Add address sanitizer")
if(ADRESS_SANITIZER)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

option(LEAK_SANITIZER "Add leak sanitizer")
if(LEAK_SANITIZER)
  add_compile_options(-fsanitize=leak)
  add_link_options(-fsanitize=leak)
endif()

option(UNDEFINED_SANITIZER "Add undefined sanitizer")
if(UNDEFINED_SANITIZER)
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif()

option(WITH_COVERAGE "Measure code coverage")
if(WITH_COVERAGE)
  add_compile_options(--coverage)
  add_link_options(--coverage)
endif()

option(WITH_TESTS "Build tests" YES)
if(WITH_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
  include(CTest)
endif()

add_subdirectory(lib/errors)
add_subdirectory(lib/utils)
add_subdirectory(lib/contract)

add_executable(main src/main.c)
target_link_libraries(main contract)

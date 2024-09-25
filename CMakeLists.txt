cmake_minimum_required(VERSION 3.21)
project(osw)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCE_DIR "C:/src/GitHub/osw")

add_subdirectory(allocator)
add_subdirectory(logger)
add_subdirectory(associative_container)
add_subdirectory(common)
add_subdirectory(main)
add_subdirectory(containers)
add_subdirectory(commands)
add_subdirectory(chain_of_responsibility)
add_subdirectory(dialog)
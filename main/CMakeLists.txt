cmake_minimum_required(VERSION 3.21)

add_executable(osw_main main.cpp)

set(PROJECT_SOURCE_DIR "C:/src/GitHub/osw")

target_include_directories(
        osw_main
        PUBLIC
        ../containers
        ../commands
        ../chain_of_responsibility
        ../containers
        ../containers/include
)

target_link_libraries(
        osw_main
        PUBLIC
        osw_containers
        osw_commands
        osw_chain_of_responsibility
)
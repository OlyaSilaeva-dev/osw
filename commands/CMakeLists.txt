cmake_minimum_required(VERSION 3.21)

add_library(osw_commands validation.h command.h)

target_include_directories(
        osw_commands
        PUBLIC
        ../containers
)

target_link_libraries(
        osw_commands
        PUBLIC
        osw_containers
)
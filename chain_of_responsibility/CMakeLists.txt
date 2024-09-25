cmake_minimum_required(VERSION 3.21)

add_library(osw_chain_of_responsibility chain_of_responsibility.h handler.h)

target_include_directories(
        osw_chain_of_responsibility
        PUBLIC
        ../commands
        ../dialog
        ../containers
)

target_link_libraries(
        osw_chain_of_responsibility
        PUBLIC
        osw_commands
        osw_containers
        osw_dialog
)
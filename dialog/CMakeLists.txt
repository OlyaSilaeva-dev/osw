cmake_minimum_required(VERSION 3.21)

add_library(osw_dialog dialog.h)

target_include_directories(
        osw_dialog
        PUBLIC
        ../commands
)

target_link_libraries(
        osw_dialog
        PUBLIC
        osw_commands
)
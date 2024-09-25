cmake_minimum_required(VERSION 3.21)
project(osw_containers)

add_library(osw_containers include/collection.h include/scheme.h include/pool.h include/database.h include/student.h include/container.h)

#add_subdirectory(include)
#add_subdirectory(tests)

target_include_directories(
        osw_containers
        PUBLIC
        ../allocator
        ../allocator/allocator_buddies_system
        ../allocator/allocator_sorted_list
        ../allocator/allocator_global_heap
        ../allocator/allocator_boundary_tags
        ../allocator/allocator_red_black_tree
        ../logger
        ../associative_container/search_tree/indexing_tree/b_tree
)

target_link_libraries(
        osw_containers
        PUBLIC
        mp_os_lggr_lggr
        mp_os_allctr_allctr
        mp_os_allctr_allctr_bndr_tgs
        mp_os_allctr_allctr_bdds_sstm
        mp_os_allctr_allctr_glbl_hp
        mp_os_allctr_allctr_srtd_lst
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr
)
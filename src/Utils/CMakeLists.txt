target_sources(RPG
        PUBLIC
        base_structs.h
        dynamic_quad_tree.h
        quad_tree_controller.h
        timer.h
        utilities.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})

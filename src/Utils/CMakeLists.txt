target_sources(RPG_game
        PUBLIC
        base_structs.h
        dynamic_quad_tree.h
        quad_tree_controller.h
        timer.h
        utilities.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})

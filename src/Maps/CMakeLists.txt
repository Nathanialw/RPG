target_sources(RPG_game
        PUBLIC
        isometric.h
        map.h
        procedural_generator.h
        scene.h
        world.h
        world_grid.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(RPG
        PUBLIC
        map.h
        procedural_generator.h
        scene.h
        world.h
        world_grid.h
        cave.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})

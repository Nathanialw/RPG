target_sources(RPG
        PUBLIC
        World/map.h
        Procedural_Generation/procedural_generator.h
        World/scene.h
        World/world.h
        World/world_grid.h
        World/cave.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})

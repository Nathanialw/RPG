target_sources(RPG
        PUBLIC
        World/map.h
        Procedural_Generation/number_generator.h
        World/scene.h
        World/world.h
        World/world_grid.h
        World/cave.h
        World/town.h
        Fog_Of_War/fog_of_war.h
        Lighting/lighting.h
        Lighting/line_of_sight.h
        Lighting/shadows.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})

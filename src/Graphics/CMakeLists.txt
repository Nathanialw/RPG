target_sources(RPG_game
        PUBLIC
        combat_graphics.h
        graphics.h
        rendering.h
        rendering_components.h
        shadow_casting.h
        sprite_parser.h
        sprite_sheets.h
        sprites.h
        spritesheet_structs.h
        texture_packer.h
        texture_packer_item.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})


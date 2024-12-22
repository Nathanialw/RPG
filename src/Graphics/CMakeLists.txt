target_sources(RPG
        PUBLIC
        combat_graphics.h
        graphics.h
        Rendering/rendering.h
        ../Components/rendering_components.h
        shadow_casting.h
        XML_Parsers/sprite_parser.h
        sprite_sheets.h
        sprites.h
        XML_Parsers/spritesheet_structs.h
        XML_Parsers/texture_packer.h
        XML_Parsers/texture_packer_item.h
        blood.h
        portraits.h
        icons.h
        colors.h
        textures.h
        Rendering/cox_rendering.h
)

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})


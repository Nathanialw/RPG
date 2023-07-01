target_sources(RPG_game
        PUBLIC
        camera.h
        event_handler.h
        events.h
        input_control.h
        joystick.h
        movement.cpp
        movement.h
        pause.h
        player_control.h
)

target_include_directories(RPG_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})

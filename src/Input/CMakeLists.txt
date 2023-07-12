target_sources(RPG
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

target_include_directories(RPG PUBLIC ${CMAKE_CURRENT_LIST_DIR})

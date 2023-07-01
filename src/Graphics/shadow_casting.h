#include "SDL2/SDL.h"
#include "components.h"



namespace Shadow_Casting {

    void Get_Positions(entt::registry &zone) {
        //get Posiiton and Radius of all targets that are Renderable
        auto view = zone.view<Component::Position, Component::Radius, Component::Renderable>();
        for (auto entity : view) {
            auto [position, radius, renderable] = view.get<Component::Position, Component::Radius, Component::Renderable>(entity);

            //get player position
            auto players_view = zone.view<Component::Position, Component::Radius, Component::Input>();
            for (auto player: players_view) {
                auto [player_position, player_radius] = players_view.get<Component::Position, Component::Radius>(player);

                //find the points perpendicular to the player

            }
        }
    }
}
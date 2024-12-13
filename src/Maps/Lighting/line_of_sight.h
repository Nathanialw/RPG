//
// Created by desktop on 12/11/24.
//
#pragma once

#include "components.h"
#include "Input/camera.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include "Collision/collision.h"
#include "shadows.h"

namespace Line_Of_Sight {
    Uint8 close = 250;
    Uint8 far = 0;

    void Update_Close(int value) {
        close += value;
        if (close >= 255)
            close = 255;
        if (close <= 0)
            close = 0;
    }

    void Update_Far(int value) {
        far += value;
        if (far >= 255)
            far = 255;
        if (far <= 0)
            far = 0;
    }

    void Draw_Shapes(std::vector<tmx::Vector2<float>> &pointVec, SDL_FRect &visionRect, Component::Position screenTargetPosition) {
        if (pointVec.size() == 3) {
            SDL_Vertex triangle[3] = {
                    {screenTargetPosition.x + pointVec[0].x, screenTargetPosition.y + pointVec[0].y, Color::Set_Color_With_Alpha(Color::yellowMagical, 100)},
                    {screenTargetPosition.x + pointVec[1].x, screenTargetPosition.y + pointVec[1].y, Color::Set_Color_With_Alpha(Color::yellowMagical, 100)},
                    {screenTargetPosition.x + pointVec[2].x, screenTargetPosition.y + pointVec[2].y, Color::Set_Color_With_Alpha(Color::yellowMagical, 100)},
            };

            if (SDL_RenderGeometry(Graphics::renderer, nullptr, triangle, 3, nullptr, 0) != 0)
                std::cerr << "Failed to render geometry: " << SDL_GetError() << std::endl;
            return;
        }

        if (pointVec.size() == 4) {
            //create a rec from the points, it is unknown which point is which
            //find the top left point
            SDL_FPoint topLeft = {INFINITY, INFINITY};
            SDL_FPoint bottomRight = {0.0f, 0.0f};
            for (int i = 0; i < pointVec.size(); i++) {
                if (pointVec[i].x + 256.0f < topLeft.x)
                    topLeft.x = pointVec[i].x + 256.0f;
                if (pointVec[i].y + 256.0f < topLeft.y)
                    topLeft.y = pointVec[i].y + 256.0f;
                if (pointVec[i].x + 256.0f > bottomRight.x)
                    bottomRight.x = pointVec[i].x + 256.0f;
                if (pointVec[i].y + 256.0f > bottomRight.y)
                    bottomRight.y = pointVec[i].y + 256.0f;
            }
            //find the height
            float height = bottomRight.y - topLeft.y;
            //find the width
            float width = bottomRight.x - topLeft.x;


            //draw the LoS rect
            SDL_FRect rect = {screenTargetPosition.x + topLeft.x - 256.0f, screenTargetPosition.y + topLeft.y - 256.0f, width, height};
            Color::Set_Render_Draw_Color(Graphics::renderer, Color::cyan, 100);
            SDL_RenderFillRectF(Graphics::renderer, &rect);

            //draw the intersection of the visionRect and the LoS rect
            if (Utilities::Rect_Intersect(visionRect, rect)) {
                SDL_FRect overlapRect = Utilities::Get_Rect_Intersect(visionRect, rect);
                Color::Set_Render_Draw_Color(Graphics::renderer, Color::blue, 100);
                SDL_RenderFillRectF(Graphics::renderer, &overlapRect);
            }
            return;
        }
    }

    void Draw_Edges(std::vector<tmx::Vector2<float>> &pointVec, Component::Position screenPosition, Component::Position screenTargetPosition, Component::Camera &camera) {

        //draw edges of collision polygons
        Color::Set_Render_Draw_Color(Graphics::renderer, Color::red, 100);
        for (int i = 0; i < pointVec.size(); i++)
            SDL_RenderDrawLineF(Graphics::renderer,
                                screenTargetPosition.x + pointVec[i].x,
                                screenTargetPosition.y + pointVec[i].y,
                                screenTargetPosition.x + pointVec[(i + 1) % pointVec.size()].x,
                                screenTargetPosition.y + pointVec[(i + 1) % pointVec.size()].y);

        //draw lines from player to vertices
        Color::Set_Render_Draw_Color(Graphics::renderer, Color::green, 100);
        for (int i = 0; i < pointVec.size(); i++) {
            SDL_FPoint point = {screenTargetPosition.x + pointVec[i].x, screenTargetPosition.y + pointVec[i].y};
            if (Camera_Control::Point_On_Screen(point, camera))
                SDL_RenderDrawLineF(Graphics::renderer, screenPosition.x, screenPosition.y, point.x, point.y);
        }
    }

    Uint8 Get_Darkness_By_Distance(float x, float y, float lightRadiusF, float lightStr, float height, float width, Uint8 maxDarkness, Uint8 minDarkness) {
        float dx = (x - (width * 0.5f)) / (lightStr * 0.75f);
        float dy = (y - (height * 0.5f)) / (lightStr * 0.5f);
        float distance = (dx * dx) + (dy * dy);

        auto alpha = (minDarkness + (sqrt(distance) * (maxDarkness - minDarkness) / (lightRadiusF * 0.5f)));

        std::cout << "Distance: " << distance << " Alpha: " << alpha << std::endl;
        return alpha;
    }


    void Draw(entt::registry &zone, Component::Camera &camera, float lightRadiusF, float lightStr, Component::Position screenPosition, SDL_FRect visionRect) {
        std::vector<Shadows::sEdge> vecEdges;

        //top
        vecEdges.push_back({visionRect.x, visionRect.y, visionRect.x + visionRect.w, visionRect.y});
        //right
        vecEdges.push_back({visionRect.x + visionRect.w, visionRect.y, visionRect.x + visionRect.w, visionRect.y + visionRect.h});
        //bottom
        vecEdges.push_back({visionRect.x + visionRect.w, visionRect.y + visionRect.h, visionRect.x, visionRect.y + visionRect.h});
        //left
        vecEdges.push_back({visionRect.x, visionRect.y + visionRect.h, visionRect.x, visionRect.y});

        auto view = zone.view<Component::Position, Collision_Component::Collider_Data, Component::Entity_Type>();
        for (auto entity: view) {
            if (view.get<Component::Entity_Type>(entity) != Component::Entity_Type::tile)
                continue;

            //if the tile does not intersect with the screen
            auto tilePosition = view.get<Component::Position>(entity);
            Component::Position screenTargetPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, tilePosition);
            SDL_FRect tileBB = {screenTargetPosition.x, screenTargetPosition.y, 512.0f, 512.0f};

            if (!Utilities::Rect_Intersect(visionRect, tileBB))
                continue;

            screenTargetPosition.x += 256.0f;
            screenTargetPosition.y += 256.0f;

            auto &colliderData = view.get<Collision_Component::Collider_Data>(entity);
            auto &coll = Collision_Component::polygonColliders.at(colliderData.name).colliders;

            for (auto &pointVec: coll.pointVecs)
                for (int i = 0; i < pointVec.size(); i++)
                    vecEdges.push_back({
                                               screenTargetPosition.x + pointVec[i].x,
                                               screenTargetPosition.y + pointVec[i].y,
                                               screenTargetPosition.x + pointVec[(i + 1) % pointVec.size()].x,
                                               screenTargetPosition.y + pointVec[(i + 1) % pointVec.size()].y});
        }

        Shadows::CalculateVisibilityPolygon(vecEdges, screenPosition.x, screenPosition.y, lightStr * (lightRadiusF * 0.5f));

        if (Shadows::vecVisibilityPolygonPoints.empty())
            return;

        for (int i = 0; i < Shadows::vecVisibilityPolygonPoints.size(); i++) {
            auto x1 = get<1>(Shadows::vecVisibilityPolygonPoints[i]);
            auto y1 = get<2>(Shadows::vecVisibilityPolygonPoints[i]);
            auto x2 = get<1>(Shadows::vecVisibilityPolygonPoints[(i + 1) % Shadows::vecVisibilityPolygonPoints.size()]);
            auto y2 = get<2>(Shadows::vecVisibilityPolygonPoints[(i + 1) % Shadows::vecVisibilityPolygonPoints.size()]);

            SDL_Vertex triangle[3] = {
                    {screenPosition.x - visionRect.x, screenPosition.y - visionRect.y, Color::Set_Color_With_Alpha(Color::black, close)},
                    {x1 - visionRect.x,               y1 - visionRect.y,               Color::Set_Color_With_Alpha(Color::black, close)},
                    {x2 - visionRect.x,               y2 - visionRect.y,               Color::Set_Color_With_Alpha(Color::black, close)}
            };

            if (SDL_RenderGeometry(Graphics::renderer, nullptr, triangle, 3, nullptr, 0) != 0)
                std::cerr << "Failed to render geometry: " << SDL_GetError() << std::endl;
        }
    }

    void Line_Of_Sight(entt::registry &zone, Component::Camera &camera, float lightRadiusF, SDL_Texture *texture) {

        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        auto player_view = zone.view<Component::Position, Component::Renderable, Component::Light_Radius>();
        for (auto player: player_view) {
            auto screenPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, player_view.get<Component::Position>(player));
            auto lightStr = player_view.get<Component::Light_Radius>(player).lightRadius;

            SDL_FRect visionRect = {
                    screenPosition.x - ((lightStr * (lightRadiusF * 0.75f)) * 0.5f),
                    screenPosition.y - ((lightStr * (lightRadiusF * 0.5f)) * 0.5f),
                    lightStr * (lightRadiusF * 0.75f),
                    lightStr * (lightRadiusF * 0.5f)
            };

            Color::Set_Render_Draw_Color(Graphics::renderer, Color::blue, 100);
            {
                SDL_SetRenderTarget(Graphics::renderer, texture);
                auto view = zone.view<Component::Position, Component::Radius, Component::Renderable>();
                for (auto entity: view) {
                    auto screenTargetPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, view.get<Component::Position>(entity));
                    if (!Utilities::bPoint_RectIntersect(screenTargetPosition.x, screenTargetPosition.y, visionRect))
                        continue;

                    auto &radius = view.get<Component::Radius>(entity).fRadius;

                    Component::Position distance = {screenPosition.x - screenTargetPosition.x, screenPosition.y - screenTargetPosition.y};
                    Component::Position perpendicular = {-distance.y, distance.x};

                    //perpendicular
                    float length = sqrt(perpendicular.x * perpendicular.x + perpendicular.y * perpendicular.y);
                    perpendicular.x = (perpendicular.x / length) * radius;
                    perpendicular.y = (perpendicular.y / length) * radius;

                    std::pair<Component::Position, Component::Position> points = {
                            {screenTargetPosition.x + perpendicular.x, screenTargetPosition.y + perpendicular.y},
                            {screenTargetPosition.x - perpendicular.x, screenTargetPosition.y - perpendicular.y},
                    };

                    //calculate intersection with the rectangle
                    auto extendToEdge = [](Component::Position start, Component::Position end, SDL_FRect rect) -> Component::Position {
                        float t1 = (rect.x - start.x) / (end.x - start.x);
                        float t2 = (rect.x + rect.w - start.x) / (end.x - start.x);
                        float t3 = (rect.y - start.y) / (end.y - start.y);
                        float t4 = (rect.y + rect.h - start.y) / (end.y - start.y);

                        float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
                        float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

                        if (tmax < 0 || tmin > tmax) {
                            return end; // No intersection
                        }

                        float t = (tmin < 0) ? tmax : tmin;
                        return {start.x + t * (end.x - start.x), start.y + t * (end.y - start.y)};
                    };

                    //extend the lines to the edges of the visionRect
                    points.first = extendToEdge(screenPosition, points.first, visionRect);
                    points.second = extendToEdge(screenPosition, points.second, visionRect);

                    //extend the lines to the edges of the visionRect
                    points.first = extendToEdge(screenPosition, points.first, visionRect);
                    points.second = extendToEdge(screenPosition, points.second, visionRect);

                    SDL_Vertex triangle[3] = {
                            {screenTargetPosition.x, screenTargetPosition.y, Color::Set_Color_With_Alpha(Color::black, close)},
                            {points.first.x,         points.first.y,         Color::Set_Color_With_Alpha(Color::black, far)},
                            {points.second.x,        points.second.y,        Color::Set_Color_With_Alpha(Color::black, far)},
                    };

                    if (SDL_RenderGeometry(Graphics::renderer, nullptr, triangle, 3, nullptr, 0) != 0)
                        std::cerr << "Failed to render geometry: " << SDL_GetError() << std::endl;

//                    SDL_RenderDrawLineF(Graphics::renderer, screenPosition.x, screenPosition.y, points.first.x, points.first.y);
//                    SDL_RenderDrawLineF(Graphics::renderer, screenPosition.x, screenPosition.y, points.second.x, points.second.y);
                }

                SDL_SetRenderTarget(Graphics::renderer, nullptr);
                Color::Set_Render_Draw_Color(Graphics::renderer);
            };
        }
    }

}
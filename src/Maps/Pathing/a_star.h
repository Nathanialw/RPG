#pragma once

#include "Debug/debug_components.h"
#include "Input/mouse_control.h"
#include "Maps/World/world.h"
#include "Maps/World/world_update.h"

namespace A_Star {

    struct sNode {
        bool bObstacle = false;
        bool bVisited = false;
        float fGlobalGoal{};
        float fLocalGoal{};
        //position
        int x{};
        int y{};
        std::vector<sNode *> vecNeighbours;
        sNode *parent{};
    };

    const int scale = 16;
    const int nMapWidth = World_Data::REGION_SIZE * scale;
    const int nMapHeight = World_Data::REGION_SIZE * scale;

    std::array<sNode, nMapWidth * nMapHeight> nodes;

    sNode *nodeStart = nullptr;
    sNode *nodeEnd = nullptr;
    int nNodeSize = World::size.width / scale;
    int nNodeBorder = nNodeSize / 4;

    void Update(entt::registry &zone) {
        auto view = zone.view<Component::Position, Action_Component::Action, Component::Interaction_Rect>();
        for (auto entity: view) {
            auto action = view.get<Action_Component::Action>(entity);
            auto rect = view.get<Component::Interaction_Rect>(entity).rect;

            //if the interation rect intersects set it as impassable

            //emplace all static objects
            if (action.state == Action_Component::isStatic) {

                float x = rect.x;
                float y = rect.y;

                for (int l = rect.w; l > -nNodeSize / 2; l -= nNodeSize) {
                    for (int h = rect.h; h > -nNodeSize / 2; h -= nNodeSize) {
                        nodes[(int(y / nNodeSize) * nMapWidth) + int(x / nNodeSize)].bObstacle = true;
                        y += nNodeSize;
                    }
                    x += nNodeSize;
                    y = rect.y;
                }
            }
        }
    }

    bool Init() {
        for (int x = 0; x < nMapWidth; x++) {
            for (int y = 0; y < nMapHeight; y++) {
                nodes[y * nMapWidth + x].x = x;
                nodes[y * nMapWidth + x].y = y;
                nodes[y * nMapWidth + x].bObstacle = false;
                nodes[y * nMapWidth + x].parent = nullptr;
                nodes[y * nMapWidth + x].bVisited = false;
                nodes[y * nMapWidth + x].vecNeighbours.clear();
            }
        }

        for (int x = 0; x < nMapWidth; x++)
            for (int y = 0; y < nMapHeight; y++) {
                if (y > 0)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
                if (y < nMapHeight - 1)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
                if (x > 0)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
                if (x < nMapWidth - 1)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

                // We can also connect diagonally
                if (y > 0 && x > 0)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
                if (y < nMapHeight - 1 && x > 0)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
                if (y > 0 && x < nMapWidth - 1)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
                if (y < nMapHeight - 1 && x < nMapWidth - 1)
                    nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
            }

        // Manually position the start and end markers so they are not nullptr
        nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
        nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];

        return true;
    }

    bool Solve_AStar(const Component::Position &position, const Component::Position &target, std::vector<f2> &path) {
        // Reset Navigation Graph - default all node states
        nodeStart = &nodes[(int(position.y / nNodeSize) * nMapWidth) + int(position.x / nNodeSize)];
        nodeEnd = &nodes[(int(target.y / nNodeSize) * nMapWidth) + int(target.x / nNodeSize)];

        int numCellsToCheck;
        (nodeEnd->bObstacle) ? numCellsToCheck = 3 : numCellsToCheck = 255;

        for (int x = 0; x < nMapWidth; x++)
            for (int y = 0; y < nMapHeight; y++) {
                nodes[y * nMapWidth + x].bVisited = false;
                nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
                nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
                nodes[y * nMapWidth + x].parent = nullptr;// No parents
            }

        auto distance = [](sNode *a, sNode *b)// For convenience
        {
            return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
        };

        auto heuristic = [distance](sNode *a, sNode *b)// So we can experiment with heuristic
        {
            return distance(a, b);
        };

        // Setup starting conditions
        sNode *nodeCurrent = nodeStart;
        nodeStart->fLocalGoal = 0.0f;
        nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

        // Add start node to not tested list - this will ensure it gets tested.
        // As the algorithm progresses, newly discovered nodes get added to this
        // list, and will themselves be tested later
        std::list<sNode *> listNotTestedNodes;
        listNotTestedNodes.push_back(nodeStart);

        // if the not tested list contains nodes, there may be better paths
        // which have not yet been explored. However, we will also stop
        // searching when we reach the target - there may well be better
        // paths but this one will do - it wont be the longest.
        int i = 0;
        while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
        {
            if (i > numCellsToCheck) {
                sNode *k = nodeCurrent;
                while (k != nodeStart) {
                    f2 cell = {(float) k->x, (float) k->y};
                    path.emplace_back(cell);
                    k = k->parent;
                    if (k == nullptr) return false;
                }
                return false;
            }
            i++;
            // Sort Untested nodes by global goal, so lowest is first
            listNotTestedNodes.sort([](const sNode *lhs, const sNode *rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

            // Front of listNotTestedNodes is potentially the lowest distance node. Our
            // list may also contain nodes that have been visited, so ditch these...
            while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
                listNotTestedNodes.pop_front();

            // ...or abort because there are no valid nodes left to test
            if (listNotTestedNodes.empty())
                break;

            nodeCurrent = listNotTestedNodes.front();
            nodeCurrent->bVisited = true;// We only explore a node once


            // Check each of this node's neighbours...
            for (auto nodeNeighbour: nodeCurrent->vecNeighbours) {
                // ... and only if the neighbour is not visited and is
                // not an obstacle, add it to NotTested List
                if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
                    listNotTestedNodes.push_back(nodeNeighbour);

                // Calculate the neighbours potential lowest parent distance
                float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

                // If choosing to path through this node is a lower distance than what
                // the neighbour currently has set, update the neighbour to use this node
                // as the path source, and set its distance scores as necessary
                if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
                    nodeNeighbour->parent = nodeCurrent;
                    nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                    // The best path length to the neighbour being tested has changed, so
                    // update the neighbour's score. The heuristic is used to globally bias
                    // the path algorithm, so it knows if its getting better or worse. At some
                    // point the algo will realise this path is worse and abandon it, and then go
                    // and search along the next best path.
                    nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
                }
            }
        }

        sNode *k = nodeEnd;
        while (k != nodeStart) {
            path.push_back({(float) k->x, (float) k->y});
            k = k->parent;
            if (k == nullptr) break;
        }

        return true;
    }

    bool Draw(entt::registry &zone, Component::Camera &camera) {

        if (Debug::settings[Debug::Settings::showPathing]) {
            for (int x = (int) camera.screen.x / World_Data::REGION_SIZE; x < (camera.screen.x + camera.screen.w) / World_Data::REGION_SIZE; x++)
                for (int y = (int) camera.screen.y / World_Data::REGION_SIZE; y < (camera.screen.y + camera.screen.h) / World_Data::REGION_SIZE; y++) {

                    SDL_FRect rect = {
                            (float) (x * nNodeSize) + nNodeBorder,
                            (float) (y * nNodeSize) + nNodeBorder,
                            (float) nNodeSize - (nNodeBorder * 2.0f),
                            (float) nNodeSize - (nNodeBorder * 2.0f)};
                    rect = Utilities::World_To_ScreenF(rect, camera.screen);

                    if (nodes[y * nMapWidth + x].bVisited) {
                        Graphics::Render_FRect(Graphics::itemBorderMagic, &rect);
                    } else if (&nodes[y * nMapWidth + x] == nodeStart) {
                        Graphics::Render_FRect(Graphics::itemBorderCommon, &rect);
                    } else if (&nodes[y * nMapWidth + x] == nodeEnd) {
                        Graphics::Render_FRect(Graphics::itemBorderEite, &rect);
                    } else if (nodes[y * nMapWidth + x].bObstacle) {
                        Graphics::Render_FRect(Graphics::itemBorderRare, &rect);
                    }
                }

            auto view = zone.view<Component::Pathing>();
            for (auto entity: view) {
                auto &pathing = view.get<Component::Pathing>(entity).path;
                if (pathing.size() > 2) {
                    for (int i = 0; i < pathing.size() - 1; ++i) {
                        SDL_SetRenderDrawColor(Graphics::renderer, 0, 125, 125, 255);
                        SDL_RenderDrawLineF(
                                Graphics::renderer,
                                (pathing[i].x * nNodeSize + nNodeSize / 2) - camera.screen.x,
                                (pathing[i].y * nNodeSize + nNodeSize / 2) - camera.screen.y,
                                (pathing[i + 1].x * nNodeSize + nNodeSize / 2) - camera.screen.x,
                                (pathing[i + 1].y * nNodeSize + nNodeSize / 2) - camera.screen.y);
                    }
                }
            }

            // Draw Path by starting ath the end, and following the parent node trail
            // back to the start - the start node will not have a parent path to follow
            //      if (nodeEnd != nullptr) {
            //        sNode *p = nodeEnd;
            //        while (p->parent != nullptr) {
            //          SDL_SetRenderDrawColor(Graphics::renderer, 0, 125, 125, 255);
            //          SDL_RenderDrawLineF(
            //              Graphics::renderer,
            //              (p->x * nNodeSize + nNodeSize / 2) - camera.screen.x,
            //              (p->y * nNodeSize + nNodeSize / 2) - camera.screen.y,
            //              (p->parent->x * nNodeSize + nNodeSize / 2) - camera.screen.x,
            //              (p->parent->y * nNodeSize + nNodeSize / 2) - camera.screen.y);
            //
            //          // Set next node to this node's parent
            //          p = p->parent;
            //        }
            //      }
        }
        return true;
    }

    f2 Move_To(entt::registry &zone, const entt::entity &entity_ID, const Component::Position &entityPosition, const Component::Position &targetPosition) {
        auto pathing = zone.emplace_or_replace<Component::Pathing>(entity_ID);
        A_Star::Solve_AStar(entityPosition, targetPosition, pathing.path);

        if (pathing.path.empty() || pathing.path.size() <= 1) {
            Utilities::Log("In target Node, moving directly");
            return {targetPosition.x, targetPosition.y};
        }
        //    int cell = 1;
        //    if (pathing.path.size() > 1)

        int cell = 2;

        float x = (pathing.path[pathing.path.size() - cell].x * A_Star::nNodeSize) + (A_Star::nNodeSize / 2.0f);
        float y = (pathing.path[pathing.path.size() - cell].y * A_Star::nNodeSize) + (A_Star::nNodeSize / 2.0f);

        return {x, y};
    }

    void Draw_Paths(entt::registry &zone, Component::Camera &camera) {
        //    auto view = zone.view<Component::A_Star>();
        //    for (auto entity: view) {
        //      auto path = view.get<Component::A_Star>(entity).path;
        //
        //      if (path.size() >= 2) {
        //        for (int i = 0; i < path.size() - 1; ++i) {
        //          SDL_SetRenderDrawColor(Graphics::renderer, 0, 125, 125, 255);
        //          SDL_RenderDrawLineF(
        //              Graphics::renderer,
        //              (path[i].x * nNodeSize + nNodeSize / 2) - camera.screen.x,
        //              (path[i].y * nNodeSize + nNodeSize / 2) - camera.screen.y,
        //              (path[i + 1].x * nNodeSize + nNodeSize / 2) - camera.screen.x,
        //              (path[i + 1].y * nNodeSize + nNodeSize / 2) - camera.screen.y);
        //        }
        //      }
        //    }
    }

}// namespace A_Star
// maybe preset maps where the cell size is the size of the entity (ie. radius of 20 would use the grid with a cell size of 20x20)
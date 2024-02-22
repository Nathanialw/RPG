#pragma once
#include "dynamic_entity_loader.h"
#include "mouse_control.h"
#include "world.h"

// have a grid of the whole map
// set all cells that overlap with a collision box to impassable

//collision radii
//32
//64
//128
//256
//512
namespace Pathing {
  //  const int size32 = World::size.width * REGION_SIZE / 32;
  //  const int size64 = World::size.width * REGION_SIZE / 64;
  //  const int size128 = World::size.width * REGION_SIZE / 128;
  //  const int size256 = World::size.width * REGION_SIZE / 256;
  //
  //  std::array<std::array<bool, size32>, size32> passableTerrain32;
  //  std::array<std::array<bool, size64>, size64> passableTerrain64;
  //  std::array<std::array<bool, size128>, size128> passableTerrain128;
  //  std::array<std::array<bool, size256>, size256> passableTerrain256;
  //
  //  void asd(std::array<std::array<bool, size32>, size32> passableTerrain) {
  //    for (int i = 0; i < passableTerrain.size(); ++i) {
  //      for (int j = 0; j < passableTerrain[i].size(); ++j) {
  //        //check for a collision with any static objects to set map defaults
  //
  //        //check for a collision with any dynamic objects every frame
  //        //      passableTerrain[i][j];
  //      }
  //    }
  //  }

  struct sNode {
    bool bObstacle = false;
    bool bVisited = false;
    float fGlobalGoal;
    float fLocalGoal;
    //position
    int x;
    int y;
    std::vector<sNode *> vecNeighbours;
    sNode *parent;
  };

  int scale = 8;

  sNode *nodes = nullptr;
  int nMapWidth = REGION_SIZE * scale;
  int nMapHeight = REGION_SIZE * scale;

  sNode *nodeStart = nullptr;
  sNode *nodeEnd = nullptr;
  int nNodeSize = World::size.width / scale;
  int nNodeBorder = nNodeSize / 4;

  bool Init() {
    nodes = new sNode[nMapWidth * nMapHeight];

    for (int x = 0; x < nMapWidth; x++) {
      for (int y = 0; y < nMapHeight; y++) {
        nodes[y * nMapWidth + x].x = x;
        nodes[y * nMapWidth + x].y = y;
        nodes[y * nMapWidth + x].bObstacle = false;
        nodes[y * nMapWidth + x].parent = nullptr;
        nodes[y * nMapWidth + x].bVisited = false;
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

  bool Solve_AStar(Component::Position &position, Component::Position &target, std::vector<f2> &path) {
    // Reset Navigation Graph - default all node states
    nodeStart = &nodes[(int(position.y / nNodeSize) * nMapWidth) + int(position.x / nNodeSize)];
    nodeEnd = &nodes[(int(target.y / nNodeSize) * nMapWidth) + int(target.x / nNodeSize)];

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
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
    {
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
    }

    return true;
  }

  bool Draw(Component::Camera &camera) {

    // Use integer division to nicely get cursor position in node space
    int nSelectedNodeX = Mouse::iXWorld_Mouse / nNodeSize;
    int nSelectedNodeY = Mouse::iYWorld_Mouse / nNodeSize;


    //    if (m_mouse[0].bReleased)// Use mouse to draw maze, shift and ctrl to place start and end
    //    {
    //      if (nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth)
    //        if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight) {
    //          if (m_keys[VK_SHIFT].bHeld)
    //            nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
    //          else if (m_keys[VK_CONTROL].bHeld)
    //            nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
    //          else
    //            nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle = !nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle;
    //
    //          //          Solve_AStar(); // Solve in "real-time" gives a nice effect
    //        }
    //    }

    // Draw Connections First - lines from this nodes position to its
    // connected neighbour node positions
    //    Fill(0, 0, camera.screen.w, camera.screen.h, L' ');
    for (int x = 0; x < nMapWidth; x++)
      for (int y = 0; y < nMapHeight; y++) {
        for (auto n: nodes[y * nMapWidth + x].vecNeighbours) {
          SDL_RenderDrawLineF(
              Graphics::renderer,
              (x * nNodeSize + nNodeSize / 2) - camera.screen.x,
              (y * nNodeSize + nNodeSize / 2) - camera.screen.y,
              (n->x * nNodeSize + nNodeSize / 2) - camera.screen.x,
              (n->y * nNodeSize + nNodeSize / 2) - camera.screen.y);
        }
      }

    // Draw Nodes on top
    for (int x = 0; x < nMapWidth; x++)
      for (int y = 0; y < nMapHeight; y++) {

        SDL_FRect rect = {
            (float) (x * nNodeSize) + nNodeBorder,
            (float) (y * nNodeSize) + nNodeBorder,
            (float) nNodeSize - (nNodeBorder * 2.0f),
            (float) nNodeSize - (nNodeBorder * 2.0f)};
        rect = Utilities::World_To_ScreenF(rect, camera.screen);
        SDL_SetRenderDrawColor(Graphics::renderer, 255, 255, 255, 155);

        SDL_Color color = {255, 255, 255, 0};
        if (nodes[y * nMapWidth + x].bVisited)
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 255, 155);

        if (&nodes[y * nMapWidth + x] == nodeStart)
          SDL_SetRenderDrawColor(Graphics::renderer, 0, 255, 0, 155);

        if (&nodes[y * nMapWidth + x] == nodeEnd)
          SDL_SetRenderDrawColor(Graphics::renderer, 255, 0, 0, 155);

        SDL_RenderDrawRectF(Graphics::renderer, &rect);
      }


    // Draw Path by starting ath the end, and following the parent node trail
    // back to the start - the start node will not have a parent path to follow
    if (nodeEnd != nullptr) {
      sNode *p = nodeEnd;
      while (p->parent != nullptr) {
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 125, 125, 255);
        SDL_RenderDrawLineF(
            Graphics::renderer,
            (p->x * nNodeSize + nNodeSize / 2) - camera.screen.x,
            (p->y * nNodeSize + nNodeSize / 2) - camera.screen.y,
            (p->parent->x * nNodeSize + nNodeSize / 2) - camera.screen.x,
            (p->parent->y * nNodeSize + nNodeSize / 2) - camera.screen.y);

        // Set next node to this node's parent
        p = p->parent;
      }
    }

    return true;
  }


}// namespace Pathing
 // maybe preset maps where the cell size is the size of the entity (ie. radius of 20 would use the grid with a cell size of 20x20)
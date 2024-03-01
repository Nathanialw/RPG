#pragma once
#include "World/world_data.h"
#include "number_generator.h"
#include <bits/stdc++.h>
#include <chrono>
#include <stack>
#include <thread>

namespace Labyrinth {
  int m_nMazeWidth;
  int m_nMazeHeight;
  std::array<int, World_Data::REGION_SIZE * World_Data::REGION_SIZE> m_maze;
  std::array<int, World_Data::REGION_SIZE * World_Data::REGION_SIZE> labyrinth;

  // Some bit fields for convenience
  enum {
    CELL_PATH_N = 0x01,
    CELL_PATH_E = 0x02,
    CELL_PATH_S = 0x04,
    CELL_PATH_W = 0x08,
    CELL_VISITED = 0x10,
  };

  int m_nVisitedCells;
  std::stack<std::pair<int, int>> m_stack;// (x, y) coordinate pairs
  Procedural_Components::Seed seed;

  // Called by olcConsoleGameEngine
  bool Init(int state) {
    for (auto &cell: labyrinth)
      cell = 0;
    for (auto &cell: m_maze)
      cell = 0;
    // Maze parameters
    m_nMazeWidth = World_Data::REGION_SIZE;
    m_nMazeHeight = World_Data::REGION_SIZE;

    // Choose a starting cell
    seed.seed = Procedural_Generation::Create_Initial_Seed(state, state);
    int x = Procedural_Generation::Random_Int(state, m_nMazeWidth, seed);
    int y = Procedural_Generation::Random_Int(state, m_nMazeHeight, seed);

    m_stack.emplace(x, y);
    m_maze[y * m_nMazeWidth + x] = CELL_VISITED;
    m_nVisitedCells = 1;

    return true;
  }

  bool Generate_Map() {
    // Slow down for animation
    //    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Little lambda function to calculate index in a readable way
    auto offset = [&](int x, int y) {
      return (m_stack.top().second + y) * m_nMazeWidth + (m_stack.top().first + x);
    };

    // Do Maze Algorithm
    while (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight) {
      // Create a set of unvisted neighbours
      std::vector<int> neighbours;

      // North neighbour
      if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
        neighbours.push_back(0);
      // East neighbour
      if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
        neighbours.push_back(1);
      // South neighbour
      if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
        neighbours.push_back(2);
      // West neighbour
      if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
        neighbours.push_back(3);

      // Are there any neighbours available?
      if (!neighbours.empty()) {

        // change to procedural!
        int next_cell_dir = neighbours[Procedural_Generation::Random_Int(0, neighbours.size(), seed)];

        // Create a path between the neighbour and the current cell
        switch (next_cell_dir) {
          case 0:// North
            m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
            m_maze[offset(0, 0)] |= CELL_PATH_N;
            m_stack.emplace((m_stack.top().first + 0), (m_stack.top().second - 1));
            break;

          case 1:// East
            m_maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
            m_maze[offset(0, 0)] |= CELL_PATH_E;
            m_stack.emplace((m_stack.top().first + 1), (m_stack.top().second + 0));
            break;

          case 2:// South
            m_maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
            m_maze[offset(0, 0)] |= CELL_PATH_S;
            m_stack.emplace((m_stack.top().first + 0), (m_stack.top().second + 1));
            break;

          case 3:// West
            m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
            m_maze[offset(0, 0)] |= CELL_PATH_W;
            m_stack.emplace((m_stack.top().first - 1), (m_stack.top().second + 0));
            break;
        }

        m_nVisitedCells++;
      } else {
        // No available neighbours so backtrack!
        m_stack.pop();
      }
    }

    for (int x = 0; x < m_nMazeWidth; x++) {
      for (int y = 0; y < m_nMazeHeight; y++) {
        labyrinth[y * m_nMazeWidth + x] = m_maze[y * m_nMazeWidth + x] -= 17;
      }
    }
    return true;
  }
}// namespace Labyrinth
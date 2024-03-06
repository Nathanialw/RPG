#include "Maps/Procedural_Generation/Labyrinth/labyrinth.h"
#include "a_star.h"

namespace Pathing_Map {

  int Increment_Column(int y) {
    return y * World_Data::REGION_SIZE * A_Star::scale;
  }

  void Labyrinth(std::array<int, World_Data::REGION_SIZE * World_Data::REGION_SIZE> &labyrinth) {
    for (int i = 0; i < World_Data::REGION_SIZE; ++i) {
      for (int j = 0; j < World_Data::REGION_SIZE; ++j) {
        //if north cell
        int row = (j * A_Star::scale * World_Data::REGION_SIZE * A_Star::scale);
        int column = (i * A_Star::scale);
        int topLeftCorner = ((j * A_Star::scale * World_Data::REGION_SIZE * A_Star::scale) + (i * A_Star::scale));

        switch (labyrinth[(j * World_Data::REGION_SIZE) + i]) {
          case 0: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if (x == 0 || x == (A_Star::scale - 1) || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 1: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set top and bottom and left
                if (x == 0 || y == 0 || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 2: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set left and bottom and top right corner
                if (x == 0 || (x == (A_Star::scale - 1) && y == 0) || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 3: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and top
                if (x == 0 || x == (A_Star::scale - 1) || y == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 4: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if (x == (A_Star::scale - 1) || x == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 5: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == (A_Star::scale - 1) && y == (A_Star::scale - 1)) || x == 0 || y == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 6: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == (A_Star::scale - 1) && y == 0) || (x == (A_Star::scale - 1) && y == (A_Star::scale - 1)) || x == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 7: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if (x == (A_Star::scale - 1) || y == (A_Star::scale - 1) || y == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 8: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == 0) || x == (A_Star::scale - 1) || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 9: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if (y == 0 || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 10: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == 0) || (y == 0 && x == (A_Star::scale - 1)) || y == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 11: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == (A_Star::scale - 1)) || y == 0 || x == (A_Star::scale - 1)) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 12: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == 0) || x == (A_Star::scale - 1) || (x == 0 && y == (A_Star::scale - 1))) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 13: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == (A_Star::scale - 1)) || (x == 0 && y == (A_Star::scale - 1)) || y == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
          case 14: {
            for (int y = 0; y < A_Star::scale; ++y) {
              for (int x = 0; x < A_Star::scale; ++x) {
                //              set sides and bottom
                if ((x == 0 && y == 0) || (x == 0 && y == (A_Star::scale - 1)) || (x == (A_Star::scale - 1) && y == (A_Star::scale - 1)) || (x == (A_Star::scale - 1)) && y == 0) {
                  A_Star::nodes[topLeftCorner + (Increment_Column(y) + x)].bObstacle = true;
                }
              }
            }
            break;
          }
        }
      }
    }
  }
}// namespace Pathing_Map
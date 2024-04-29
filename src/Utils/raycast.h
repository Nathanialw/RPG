#pragma once


namespace Ray_Cast {

  bool Line_Of_Sight() {
    return true;
  }

  f2 vPlayer = {0.0f, 0.0f};
  i2 vMapSize = {32, 30};
  i2 vCellSize = {16, 16};
  std::vector<int> vecMap;


  bool OnUserCreate() {
    // Construct Map
    vecMap.resize(vMapSize.x * vMapSize.y);
    return true;
  }

  bool OnUserUpdate() {
    f2 vMouse = {Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse};
    f2 vMouseCell;
    vMouseCell.x = vMouse.x / vCellSize.x;
    vMouseCell.y = vMouse.y / vCellSize.y;
    i2 vCell = {(int) vMouseCell.x, (int) vMouseCell.y};// implicit cast to integer, rounds down


    // DDA Algorithm ==============================================
    // https://lodev.org/cgtutor/raycasting.html


    // Form ray cast from player into scene
    f2 vRayStart = vPlayer;
    f2 temp = {vMouseCell.x - vPlayer.x, vMouseCell.y - vPlayer.y};
    f2 vRayDir = Normalize(temp);

    // Lodev.org also explains this additional optimistaion (but it's beyond scope of video)
    // f2 vRayUnitStepSize = { abs(1.0f / vRayDir.x), abs(1.0f / vRayDir.y) };

    f2 vRayUnitStepSize = {sqrt(1 + (vRayDir.y / vRayDir.x) * (vRayDir.y / vRayDir.x)), sqrt(1 + (vRayDir.x / vRayDir.y) * (vRayDir.x / vRayDir.y))};
    i2 vMapCheck = {(int) vRayStart.x, (int) vRayStart.y};
    f2 vRayLength1D;
    i2 vStep;

    // Establish Starting Conditions
    if (vRayDir.x < 0) {
      vStep.x = -1;
      vRayLength1D.x = (vRayStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
    } else {
      vStep.x = 1;
      vRayLength1D.x = (float(vMapCheck.x + 1) - vRayStart.x) * vRayUnitStepSize.x;
    }

    if (vRayDir.y < 0) {
      vStep.y = -1;
      vRayLength1D.y = (vRayStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
    } else {
      vStep.y = 1;
      vRayLength1D.y = (float(vMapCheck.y + 1) - vRayStart.y) * vRayUnitStepSize.y;
    }

    // Perform "Walk" until collision or range check
    bool bTileFound = false;
    float fMaxDistance = 100.0f;
    float fDistance = 0.0f;
    while (!bTileFound && fDistance < fMaxDistance) {
      // Walk along shortest path
      if (vRayLength1D.x < vRayLength1D.y) {
        vMapCheck.x += vStep.x;
        fDistance = vRayLength1D.x;
        vRayLength1D.x += vRayUnitStepSize.x;
      } else {
        vMapCheck.y += vStep.y;
        fDistance = vRayLength1D.y;
        vRayLength1D.y += vRayUnitStepSize.y;
      }

      // Test tile at new test point
      if (vMapCheck.x >= 0 && vMapCheck.x < vMapSize.x && vMapCheck.y >= 0 && vMapCheck.y < vMapSize.y) {
        if (vecMap[vMapCheck.y * vMapSize.x + vMapCheck.x] == 1) {
          bTileFound = true;
        }
      }
    }

    // Calculate intersection location
    f2 vIntersection;
    if (bTileFound) {
      //      vIntersection = vRayStart + vRayDir * fDistance;
    }
  };

}// namespace Ray_Cast
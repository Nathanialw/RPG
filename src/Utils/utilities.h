#pragma once
#include "SDL2/SDL.h"
#include "math.h"
#include <iostream>

namespace Utilities {

  void Log(const auto &string) {
    std::cout << string << std::endl;
  }

  void Log(const int &string) {
    std::cout << string << std::endl;
  }

  void Log(const float &string) {
    std::cout << string << std::endl;
  }

  SDL_FRect worldToScreen(const SDL_FRect &Rect, const SDL_FRect &camera) {
    SDL_FRect screenRect = {};

    screenRect.x = Rect.x + camera.x;
    screenRect.y = Rect.y + camera.y;

    return screenRect;
  }

  SDL_FRect World_To_ScreenF(const SDL_FRect &Rect, const SDL_FRect &camera) {
    SDL_FRect screenRect = Rect;

    screenRect.x = Rect.x - camera.x;
    screenRect.y = Rect.y - camera.y;

    return screenRect;
  }

  SDL_FRect screenToWorld(const SDL_FRect &Rect, const SDL_FRect &camera) {
    SDL_FRect screenRect = Rect;

    screenRect.x = Rect.x - camera.x;
    screenRect.y = Rect.y - camera.y;

    return screenRect;
  }

  float Get_Hypotenuse(const float &length, const float &width) {
    return sqrtf((length * length) + (width * width));
  }

  bool bFRect_Intersect(const SDL_FRect &entity, const SDL_FRect &target) {
    if ((entity.y <= target.y + target.h) &&
        (entity.x <= target.x + target.w) &&
        (entity.y + entity.h >= target.y) &&
        (entity.x + entity.w >= target.x)) {
      return true;
    }
    return false;
  };

  bool bRect_Intersect(const SDL_Rect &entity, const SDL_Rect &target) {
    if ((entity.y <= target.y + target.h) &&
        (entity.x <= target.x + target.w) &&
        (entity.y + entity.h >= target.y) &&
        (entity.x + entity.w >= target.x)) {
      return true;
    }
    return false;
  };

  bool bFPoint_FRectIntersect(const SDL_FPoint &p, const SDL_FRect &r) {
    return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
  };

  bool bPoint_RectIntersect(const SDL_FPoint &p, const SDL_FRect &r) {
    return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
  };

  SDL_Rect SDL_FRect_To_SDL_Rect(const SDL_FRect &a) {
    SDL_Rect b = {};
    b.x = (int) a.x;
    b.y = (int) a.y;
    b.w = (int) a.w;
    b.h = (int) a.h;
    return b;
  }

  SDL_FRect SDL_Rect_To_SDL_FRect(const SDL_Rect &a) {
    SDL_FRect b = {};
    b.x = (float) a.x;
    b.y = (float) a.y;
    b.w = (float) a.w;
    b.h = (float) a.h;
    return b;
  }

  SDL_Point SDL_FPoint_to_Point(const SDL_FPoint &a) {
    SDL_Point b = {};
    b.x = (int) a.x;
    b.y = (int) a.y;
    return b;
  }

  SDL_FPoint SDL_FPoint_to_Point(const SDL_Point &a) {
    SDL_FPoint b = {};
    b.x = (float) a.x;
    b.y = (float) a.y;
    return b;
  }


  float Get_Direction_Point(const float &sourceX, const float &sourceY, const float &targetX, const float &targetY) {
    float a = targetX - sourceX;
    float b = targetY - sourceY;

    return atan2f(a, b);
  }

  SDL_FRect Get_FRect_From_Point_Radius(const float &radius, const float &x, const float &y) {
    float rectX = x - radius;
    float rectY = y - radius;
    float width = radius * 2.0f;
    float height = radius * 2.0f;

    SDL_FRect rect = {rectX, rectY, width, height};
    return rect;
  }

  SDL_FRect Centre_Rect_On_Position(const SDL_FRect &frect, const float &x, const float &y) {
    SDL_FRect rect;
    rect.x = x - (frect.w / 2.0f);
    rect.y = y - (frect.h / 2.0f);
    rect.w = frect.w;
    rect.h = frect.h;

    return rect;
  }

  SDL_Point Check_Collision_Rects(const SDL_Rect &rect1, const SDL_Rect &rect2) {

    if (Utilities::bRect_Intersect(rect1, rect2)) {
      SDL_Point returnRect = {};
      int xOverlap = rect1.x - rect2.x;
      int yOverlap = rect1.y - rect2.y;
      if (xOverlap == 0) {
        xOverlap = 2;
      }
      if (yOverlap == 0) {
        yOverlap = 2;
      }
      returnRect.x = (xOverlap / 2);
      returnRect.y = (yOverlap / 2);
      return returnRect;
    }
    //Log("Check_Collision_Rects() fallthrough error");
    return {0, 0};
  }


  bool bFrect_Contains_Frect(const SDL_FRect &rect, const SDL_FRect &rectWithin) {
    if (rectWithin.x > rect.x && (rectWithin.x + rectWithin.w) < (rect.x + rect.w) &&
        rectWithin.y > rect.y && (rectWithin.y + rectWithin.h) < (rect.y + rect.h)) {
      return true;
    };
    return false;

    //
    //if ((entity.y <= target.y + target.h) &&
    //	(entity.x <= target.x + target.w) &&
    //	(entity.y + entity.h >= target.y) &&
    //	(entity.x + entity.w >= target.x)) {
    //	return true;
    //}
    //return false;
  }

  SDL_FRect Scale_FRect(const SDL_FRect &clippedSprite, const float &scale) {
    SDL_FRect fScaledImage = clippedSprite;
    fScaledImage = {
        fScaledImage.x - (fScaledImage.w * scale),
        fScaledImage.y - (fScaledImage.h * scale),
        fScaledImage.w * scale,
        fScaledImage.h * scale};
    return fScaledImage;
  }

  SDL_FRect Scale_Rect(const SDL_Rect &clippedSprite, const float &scale) {
    SDL_FRect fScaledImage = SDL_Rect_To_SDL_FRect(clippedSprite);
    fScaledImage = {
        fScaledImage.x - (fScaledImage.w * scale),
        fScaledImage.y - (fScaledImage.h * scale),
        fScaledImage.w * scale,
        fScaledImage.h * scale};
    return fScaledImage;
  }

  int Get_Random_Number(const int min, const int max) {
    if (min == 0) {
      std::cout << "function Utilities::Get_Random_Number() min range 0, divide by zero error" << std::endl;
      //			return 0;
    }
    return rand() % max + min;
  }

  bool PolygonOverlap_SAT(std::vector<f2> &r1, std::vector<f2> &r2) {
    std::vector<f2> *poly1 = &r1;
    std::vector<f2> *poly2 = &r2;

    for (int shape = 0; shape < 2; shape++) {
      if (shape == 1) {
        poly1 = &r2;
        poly2 = &r1;
      }

      for (int a = 0; a < poly1->size(); a++) {
        int b = (a + 1) % poly1->size();
        f2 axisProj = {-(poly1->at(b).y - poly1->at(a).y), poly1->at(b).x - poly1->at(a).x};
        float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
        axisProj = {axisProj.x / d, axisProj.y / d};

        // Work out min and max 1D points for r1
        float min_r1 = INFINITY, max_r1 = -INFINITY;
        for (int p = 0; p < poly1->size(); p++) {
          float q = (poly1->at(p).x * axisProj.x + poly1->at(p).y * axisProj.y);
          min_r1 = std::min(min_r1, q);
          max_r1 = std::max(max_r1, q);
        }

        // Work out min and max 1D points for r2
        float min_r2 = INFINITY, max_r2 = -INFINITY;
        for (int p = 0; p < poly2->size(); p++) {
          float q = (poly2->at(p).x * axisProj.x + poly2->at(p).y * axisProj.y);
          min_r2 = std::min(min_r2, q);
          max_r2 = std::max(max_r2, q);
        }

        if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
          return false;
      }
    }

    return true;
  }

}// namespace Utilities

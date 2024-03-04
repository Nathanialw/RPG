#pragma once
#include "components.h"
#include "graphics.h"

namespace Sort {
  void RenderLines() {
    auto &zone = World::world[World::currentZone.current].zone;

    auto view = zone.view<Component::Renderable>();
    auto camera_view = zone.view<Component::Camera>();

    for (auto camera: camera_view) {
      auto &screen = camera_view.get<Component::Camera>(camera).screen;
      for (auto entity: view) {
        auto &lines = view.get<Component::Renderable>(entity).lineSegment;
        for (auto line: lines) {
          SDL_RenderDrawLineF(Graphics::renderer, line.start.x - screen.x, line.start.y - screen.y, line.end.x - screen.x, line.end.y - screen.y);
        }
      }
    }
  }

  /*
  //returns 0 if the point is ABOVE the line on the screen
  */
  int ComparePointAndLine(const Component::Position &point, const Component::Line_Segment &line) {
    if (point.y > line.start.y && point.y > line.end.y) {
      return 1;
    } else if (point.y < line.start.y && point.y < line.end.y) {
      return 0;
    } else {
      float slope = (line.end.y - line.start.y) / (line.end.x - line.start.x);
      float intercept = line.start.y - (slope * line.start.x);
      float yOnLineForPoint = (slope * point.x) + intercept;
      return point.y > yOnLineForPoint ? 1 : 0;
    }
  }

  /* Compares the position point of a polygon building to the ordering line of another poly building
   * FOR THIS TO WORK the position point must be within the collision box of every building
   * Otherwise both position points of the buildings being compared may fall below the slope of each other causing flickering
   */
  int CompareLineAndLine(const Component::Position &point, const Component::Line_Segment &line) {
    float slope = (line.end.y - line.start.y) / (line.end.x - line.start.x);
    float intercept = line.start.y - (slope * line.start.x);
    float yOnLineForPoint = (slope * point.x) + intercept;
    return point.y > yOnLineForPoint ? 1 : 0;
  }

  /*
        BECAUSE the y of 'lhs' is a lower value than the y of 'rhs';
        THEN the algo places the 'lhs' BEHIND the 'rhs'
        returns 1 to swap the positions

        SO return 1 when you want the lhs BEHIND the rhs
  */
  int Sort(const Component::Renderable &lhs, const Component::Renderable &rhs) {
    std::vector<int> g;
    if (!rhs.lineSegment.empty() && !lhs.lineSegment.empty()) {
      for (auto line: lhs.lineSegment) {
        g.emplace_back(CompareLineAndLine(rhs.point, line));
      }
      if (g[0] && g[1]) {
        return 1;
      } else {
        return 0;
      };
    }

    if (!lhs.lineSegment.empty()) {
      for (auto line: lhs.lineSegment) {
        g.emplace_back(ComparePointAndLine(rhs.point, line));
      }
      if (g[0] && g[1]) {
        return 1;
      } else {
        return 0;
      };
    }

    if (!rhs.lineSegment.empty()) {
      for (auto line: rhs.lineSegment) {
        g.emplace_back(ComparePointAndLine(lhs.point, line));
      }
      if (g[0] && g[1]) {
        return 0;
      } else {
        return 1;
      };
    }

    if (lhs.point.y < rhs.point.y) {
      if (!lhs.inside) {
        return 1;
      } else if (rhs.inside) {
        return 1;
      }
    }

    return 0;
  }

  float SortIt = 0;
  void Sort_Positions(entt::registry &zone) {
    //    Utilities::Log("Call Sort Function");
    zone.sort<Component::Renderable>([&](const Component::Renderable &lhs, const Component::Renderable &rhs) {
      return Sort(lhs, rhs);
    });
  }
}// namespace Sort
//
// Created by desktop on 12/12/24.
//
#pragma once

#include <algorithm>
#include "math.h"

namespace Shadows {
/*
 *  Stolen from:
 *  https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_ShadowCasting2D.cpp
 */

    struct sEdge {
        float sx, sy, ex, ey;
    };

    std::vector<sEdge> vecEdges;
    std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;


    void CalculateVisibilityPolygon(float ox, float oy, float radius) {
        // Get rid of existing polygon
        vecVisibilityPolygonPoints.clear();

        // For each edge in PolyMap
        for (auto &e1: vecEdges) {
            // Take the start point, then the end point (we could use a pool of
            // non-duplicated points here, it would be more optimal)
            for (int i = 0; i < 2; i++) {
                float rdx, rdy;
                rdx = (i == 0 ? e1.sx : e1.ex) - ox;
                rdy = (i == 0 ? e1.sy : e1.ey) - oy;

                float base_ang = atan2f(rdy, rdx);

                float ang = 0;
                // For each point, cast 3 rays, 1 directly at point
                // and 1 a little bit either side
                for (int j = 0; j < 3; j++) {
                    if (j == 0) ang = base_ang - 0.0001f;
                    if (j == 1) ang = base_ang;
                    if (j == 2) ang = base_ang + 0.0001f;

                    // Create ray along angle for required distance
                    rdx = radius * cosf(ang);
                    rdy = radius * sinf(ang);

                    float min_t1 = INFINITY;
                    float min_px = 0, min_py = 0, min_ang = 0;
                    bool bValid = false;

                    // Check for ray intersection with all edges
                    for (auto &e2: vecEdges) {
                        // Create line segment vector
                        float sdx = e2.ex - e2.sx;
                        float sdy = e2.ey - e2.sy;

                        if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f) {
                            // t2 is normalised distance from line segment start to line segment end of intersect point
                            float t2 = (rdx * (e2.sy - oy) + (rdy * (ox - e2.sx))) / (sdx * rdy - sdy * rdx);
                            // t1 is normalised distance from source along ray to ray length of intersect point
                            float t1 = (e2.sx + sdx * t2 - ox) / rdx;

                            // If intersect point exists along ray, and along line
                            // segment then intersect point is valid
                            if (t1 > 0 && t2 >= 0 && t2 <= 1.0f) {
                                // Check if this intersect point is closest to source. If
                                // it is, then store this point and reject others
                                if (t1 < min_t1) {
                                    min_t1 = t1;
                                    min_px = ox + rdx * t1;
                                    min_py = oy + rdy * t1;
                                    min_ang = atan2f(min_py - oy, min_px - ox);
                                    bValid = true;
                                }
                            }
                        }
                    }

                    if (bValid)// Add intersection point to visibility polygon perimeter
                        vecVisibilityPolygonPoints.push_back({min_ang, min_px, min_py});
                }
            }
        }

        // Sort perimeter points by angle from source. This will allow
        // us to draw a triangle fan.
        sort(
                vecVisibilityPolygonPoints.begin(),
                vecVisibilityPolygonPoints.end(),
                [&](const std::tuple<float, float, float> &t1, const std::tuple<float, float, float> &t2) {
                    return get<0>(t1) < get<0>(t2);
                });

    }
}
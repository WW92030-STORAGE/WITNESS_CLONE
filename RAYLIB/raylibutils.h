#ifndef RAYLIB_UTILS_H
#define RAYLIB_UTILS_H

#include <iostream>
#include <raylib.h>
// #include <rlgl.h>
#include <memory>
#include <cmath>
#include <vector>

// #include "raylibutils.h"

// DRAWING METHODS DRAWING METHODS DRAWING METHODS DRAWING METHODS

// Draw an axis aligned rectangle from two opposing points
inline void Rect2Points(double x, double y, double w, double h, Color col) {
    if (x > w || y > h) {
        Rect2Points(std::min(x, w), std::min(y, h), std::max(x, w), std::max(y, h), col);
        return;
    }
    Rectangle rect = {x, y, w - x, h - y};
    DrawRectangleRec(rect, col);
}

// Draw a rotated rectangle parallel to two points and with a specified half-thickness

inline void DrawRotatedRect(double x1, double y1, double x2, double y2, double rad, Color col) {
    if (x1 > x2) {
        DrawRotatedRect(x2, y2, x1, y1, rad, col);
        return;
    }

    double dist = (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1);
    dist = sqrt(dist);
    Rectangle rec = {0.5 * (x1 + x2), 0.5 * (y1 + y2), dist, 2 * rad};
    Vector2 origin = {0.5 * dist, rad};
    double rotation = atan2(y2 - y1, x2 - x1) * RAD2DEG;

    DrawRectanglePro(rec, origin, rotation, col);
}

// Draw a slot (a rectangle with two circles on the ends) of a specified line segment and radius (half thickness)

inline void DrawSlot(double x1, double y1, double x2, double y2, double rad, Color col) {
    DrawCircle(x1, y1, rad, col);
    DrawCircle(x2, y2, rad, col);

    DrawRotatedRect(x1, y1, x2, y2, rad, col);
}

// Draw centered text

inline void DrawCenteredText(const char* text, double xpos, double ypos, double fontsize, Color col) {
    double len = MeasureText(text, fontsize);
    DrawText(text, xpos - len / 2, ypos, fontsize, col);
}

/*

NOTATION FOR THESE METHODS

The syntax for the names is (destination)(source)
where destination is what is supposed to come out and source is what is supposed to go in. all source inputs (except real numbers) are of the same "type".

prec = pixel rectangle
rot = rotation
rsq = squared distance
vec2 = Vector2
vec3 = Vector3
vec4 = Vector4
pd = pair<double, double>
pf = pair<float, float>
vd = vector<double>
vf = vector<float>

*/

// GEOMETRY METHODS GEOMETRY METHODS GEOMETRY METHODS GEOMETRY METHODS

// Rotations
inline std::pair<double, double> rotpd(std::pair<double, double> point, std::pair<double, double> origin, double rad) {
    double dx = point.first - origin.first;
    double dy = point.second - origin.second;
    std::pair<double, double> blip = {dx * cos(rad) - dy * sin(rad), dx * sin(rad) + dy * cos(rad)};
    return {blip.first + origin.first, blip.second + origin.second};
}

inline std::pair<float, float> rotpf(std::pair<float, float> point, std::pair<float, float> origin, double rad) {
    double dx = point.first - origin.first;
    double dy = point.second - origin.second;
    std::pair<double, double> blip = {dx * cos(rad) - dy * sin(rad), dx * sin(rad) + dy * cos(rad)};
    return {float(blip.first + origin.first), float(blip.second + origin.second)};
}

// Distance
inline double rsqpd(std::pair<double, double> a, std::pair<double, double> b) {
    return (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
}

inline double rsqpf(std::pair<float, float> a, std::pair<float, float> b) {
    return (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
}

inline double rsqvec2(Vector2 a, Vector2 b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

// Inside bounding box with possible leeway

inline bool inRange(Vector2 p, Vector2 a, Vector2 b, double dx = 0, double dy = 0) {
    double x1 = std::min(a.x, b.x) - dx;
    double x2 = std::max(a.x, b.x) + dx;
    double y1 = std::min(a.y, b.y) - dy;
    double y2 = std::max(a.y, b.y) + dy;

    return (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2);
}

// NUMERICAL METHODS NUMERICAL METHODS NUMERICAL METHODS NUMERICAL METHODS

// Clamp value between two bounds
inline double clamp(double i, double L, double R) {
    if (i < std::min(L, R)) return std::min(L, R);
    if (i > std::max(L, R)) return std::max(L, R);
    return i;
}

// Sign function

inline int sgn(double x) {
    if (x == 0) return 0;
    if (x > 0) return 1;
    return -1;
}

// Python range
inline std::vector<int> range(int stop, bool include = false) {
    std::vector<int> res;
    if (include) stop++;
    for (int i = 0; i < stop; i++) res.push_back(i);
    return res;
}
inline std::vector<int> range(int start, int stop, bool include = false) {
    std::vector<int> res;
    if (stop > start) {
        if (include) stop++;
        for (int i = start; i < stop; i++) res.push_back(i);
    }
    else {
        if (include) stop--;
        for (int i = start; i > stop; i--) res.push_back(i);
    }
    return res;
}
inline std::vector<int> range(int start, int stop, int step, bool include = false) {
    std::vector<int> res;
    if (step > 0) {
        if (include) stop++;
        for (int i = start; i < stop; i += step) res.push_back(i);
    }
    else {
        if (include) stop--;
        for (int i = start; i > stop; i -= step) res.push_back(i);
    }
    return res;
}

// Get all pixels in an axis aligned rectangle spanning the corners a and b
inline std::vector<Vector2> precvec2(Vector2 a, Vector2 b) {
    std::vector<Vector2> res;
    for (auto i : range(int(a.x), int(b.x), true)) {
        for (auto j : range(int(a.y), int(b.y), true)) {
            Vector2 v = {i, j};
            res.push_back(v);
        }
    }
    return res;
}

// MISC METHODS MISC METHODS MISC METHODS MISC METHODS

inline bool equals(Vector2 v, Vector2 w) {
    return v.x == w.x && v.y == w.y;
}

// CONVERSION METHODS CONVERSION METHODS CONVERSION METHODS CONVERSION METHODS

// Convert from pairs and vectors to Vectors

inline Vector2 vec2vd(std::vector<double> v) {
    if (v.size() < 2) return {0, 0};
    return {v[0], v[1]};
}

inline Vector2 vec2vf(std::vector<float> v) {
    if (v.size() < 2) return {0, 0};
    return {v[0], v[1]};
}

inline Vector2 vec2pd(std::pair<double, double> p) {
    Vector2 res = {p.first, p.second};
    return res;
}

inline Vector2 vec2pf(std::pair<float, float> p) {
    Vector2 res = {p.first, p.second};
    return res;
}

inline Vector3 vec3vd(std::vector<double> v) {
    if (v.size() < 3) return {0, 0, 0};
    return {v[0], v[1], v[2]};
}

inline Vector3 vec3vf(std::vector<float> v) {
    if (v.size() < 3) return {0, 0, 0};
    return {v[0], v[1], v[2]};
}

inline Vector4 vec4vd(std::vector<double> v) {
    if (v.size() < 4) return {0, 0, 0, 0};
    return {v[0], v[1], v[2], v[3]};
}

inline Vector4 vec4vf(std::vector<float> v) {
    if (v.size() < 4) return {0, 0, 0, 0};
    return {v[0], v[1], v[2], v[3]};
}

// Convert from Vectors to pairs and vectors

inline std::pair<double, double> pdvec2(Vector2 v) {
    return std::pair<double, double>{v.x, v.y};
}

inline std::pair<float, float> pfvec2(Vector2 v) {
    return std::pair<float, float>{v.x, v.y};
}

inline std::vector<float> vfvec2(Vector2 v) {
    return std::vector<float>({v.x, v.y});
}

inline std::vector<double> vdvec2(Vector2 v) {
    return std::vector<double>({v.x, v.y});
}

inline std::vector<double> vdvec3(Vector3 v) {
    return std::vector<double>({v.x, v.y, v.z});
}

inline std::vector<float> vfvec3(Vector3 v) {
    return std::vector<float>({v.x, v.y, v.z});
}

inline std::vector<double> vdvec4(Vector4 v) {
    return std::vector<double>({v.x, v.y, v.z, v.w});
}

inline std::vector<float> vfvec4(Vector4 v) {
    return std::vector<float>({v.x, v.y, v.z, v.w});
}

#endif

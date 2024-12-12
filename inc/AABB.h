#pragma once
#include "core.h"

class AABB {
public:
    vec3 minExt; // Minimum extents
    vec3 maxExt; // Maximum extents

    AABB() : minExt(vec3(FLT_MAX, FLT_MAX, FLT_MAX)), maxExt(vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX)) {}

    void reset();
    void extend(const vec3& point);
    bool intersects(const AABB& other) const;
    vec3 getCenter() const;
    vec3 getSize() const;
};

#pragma once
#include "core.h"

class AABB {
public:
    vec3 minExt; // Minimum extents
    vec3 maxExt; // Maximum extents

    AABB() : minExt(vec3(FLT_MAX, FLT_MAX, FLT_MAX)), maxExt(vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX)) {}

    void reset() {
        minExt = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxExt = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }

    void extend(const vec3& point) {
        minExt = vec3::Min(minExt, point);
        maxExt = vec3::Max(maxExt, point);
    }

    bool intersects(const AABB& other) const {
        return (minExt.x <= other.maxExt.x && maxExt.x >= other.minExt.x) &&
            (minExt.y <= other.maxExt.y && maxExt.y >= other.minExt.y) &&
            (minExt.z <= other.maxExt.z && maxExt.z >= other.minExt.z);
    }

    vec3 getCenter() const {
        return (minExt + maxExt) * 0.5f;
    }

    vec3 getSize() const {
        return maxExt - minExt;
    }
};

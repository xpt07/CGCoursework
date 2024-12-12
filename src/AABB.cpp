#include "../inc/AABB.h"

void AABB::reset() {
    minExt = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    maxExt = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void AABB::extend(const vec3& point) {
    minExt = vec3::Min(minExt, point);
    maxExt = vec3::Max(maxExt, point);
}

bool AABB::intersects(const AABB& other) const {
    return (minExt.x <= other.maxExt.x && maxExt.x >= other.minExt.x) &&
        (minExt.y <= other.maxExt.y && maxExt.y >= other.minExt.y) &&
        (minExt.z <= other.maxExt.z && maxExt.z >= other.minExt.z);
}

vec3 AABB::getCenter() const {
    return (minExt + maxExt) * 0.5f;
}

vec3 AABB::getSize() const {
    return maxExt - minExt;
}

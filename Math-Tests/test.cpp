#include "pch.h"
#include <gtest/gtest.h>
#include "../inc/core.h"

// vec2 Tests
TEST(Vec2Test, Addition) {
    vec2 v1(1.0f, 2.0f);
    vec2 v2(3.0f, 4.0f);
    vec2 result = v1 + v2;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 6.0f);
}

TEST(Vec2Test, Subtraction) {
    vec2 v1(5.0f, 7.0f);
    vec2 v2(2.0f, 4.0f);
    vec2 result = v1 - v2;
    EXPECT_EQ(result.x, 3.0f);
    EXPECT_EQ(result.y, 3.0f);
}

TEST(Vec2Test, Length) {
    vec2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.getLength(), 5.0f);
}

// vec3 Tests
TEST(Vec3Test, DotProduct) {
    vec3 v1(1.0f, 2.0f, 3.0f);
    vec3 v2(4.0f, 5.0f, 6.0f);
    EXPECT_FLOAT_EQ(v1.dot(v2), 32.0f);
}

TEST(Vec3Test, CrossProduct) {
    vec3 v1(1.0f, 0.0f, 0.0f);
    vec3 v2(0.0f, 1.0f, 0.0f);
    vec3 result = v1.cross(v2);
    EXPECT_EQ(result.x, 0.0f);
    EXPECT_EQ(result.y, 0.0f);
    EXPECT_EQ(result.z, 1.0f);
}

TEST(Vec3Test, Normalize) {
    vec3 v(3.0f, 0.0f, 0.0f);
    vec3 result = v.normalize();
    EXPECT_FLOAT_EQ(result.x, 1.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
}

// vec4 Tests
TEST(Vec4Test, Addition) {
    vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    vec4 v2(4.0f, 3.0f, 2.0f, 1.0f);
    vec4 result = v1 + v2;
    EXPECT_EQ(result.x, 5.0f);
    EXPECT_EQ(result.y, 5.0f);
    EXPECT_EQ(result.z, 5.0f);
    EXPECT_EQ(result.w, 5.0f);
}

TEST(Vec4Test, DotProduct) {
    vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    vec4 v2(4.0f, 3.0f, 2.0f, 1.0f);
    EXPECT_FLOAT_EQ(v1.dot(v2), 20.0f);
}

// Matrix Tests
TEST(MatrixTest, Identity) {
    Matrix mat;
    mat.identity();
    EXPECT_FLOAT_EQ(mat.m[0], 1.0f);
    EXPECT_FLOAT_EQ(mat.m[5], 1.0f);
    EXPECT_FLOAT_EQ(mat.m[10], 1.0f);
    EXPECT_FLOAT_EQ(mat.m[15], 1.0f);
}

TEST(MatrixTest, Multiplication) {
    Matrix m1, m2;
    m1.m[0] = 1.0f; m1.m[5] = 1.0f; m1.m[10] = 1.0f; m1.m[15] = 1.0f;
    m2.m[0] = 2.0f; m2.m[5] = 2.0f; m2.m[10] = 2.0f; m2.m[15] = 2.0f;
    Matrix result = m1.mul(m2);
    EXPECT_FLOAT_EQ(result.m[0], 2.0f);
    EXPECT_FLOAT_EQ(result.m[5], 2.0f);
    EXPECT_FLOAT_EQ(result.m[10], 2.0f);
    EXPECT_FLOAT_EQ(result.m[15], 2.0f);
}

TEST(MatrixTest, Inversion) {
    Matrix mat;
    mat.m[0] = 1.0f; mat.m[5] = 1.0f; mat.m[10] = 1.0f; mat.m[15] = 1.0f;
    Matrix inv = mat.invert();
    EXPECT_FLOAT_EQ(inv.m[0], 1.0f);
    EXPECT_FLOAT_EQ(inv.m[5], 1.0f);
    EXPECT_FLOAT_EQ(inv.m[10], 1.0f);
    EXPECT_FLOAT_EQ(inv.m[15], 1.0f);
}

// Quaternion tests
TEST(QuaternionTest, Multiplication) {
    Quaternion q1(1, 0, 0, 0); // Identity quaternion
    Quaternion q2(0, 1, 0, 0); // 180-degree rotation around X-axis
    Quaternion result = q1 * q2;
    EXPECT_EQ(result.w, 0);
    EXPECT_EQ(result.x, 1);
    EXPECT_EQ(result.y, 0);
    EXPECT_EQ(result.z, 0);
}

TEST(QuaternionTest, Slerp) {
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    Quaternion result = Quaternion::slerp(q1, q2, 0.5f);
    EXPECT_FLOAT_EQ(result.w, sqrt(2.0f) / 2.0f);
    EXPECT_FLOAT_EQ(result.x, sqrt(2.0f) / 2.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
}

// Colour Tests
TEST(ColourTest, Addition) {
    Colour c1(0.1f, 0.2f, 0.3f);
    Colour c2(0.4f, 0.5f, 0.6f);
    Colour result = c1 + c2;
    EXPECT_FLOAT_EQ(result.r, 0.5f);
    EXPECT_FLOAT_EQ(result.g, 0.7f);
    EXPECT_FLOAT_EQ(result.b, 0.9f);
}

TEST(ColourTest, Multiplication) {
    Colour c(0.1f, 0.2f, 0.3f);
    Colour result = c * 2.0f;
    EXPECT_FLOAT_EQ(result.r, 0.2f);
    EXPECT_FLOAT_EQ(result.g, 0.4f);
    EXPECT_FLOAT_EQ(result.b, 0.6f);
}


#pragma once
#include <cmath>
#include <cstring>
#include <algorithm>
#include <memory.h>
#include <iostream>
#define _USE_MATH_DEFINES
#define SQ(x) (x)* (x)
#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)
constexpr auto CANVAS_WIDTH = 1024;
constexpr auto CANVAS_HEIGHT = 768;
constexpr double M_PI = 3.14159265358979323846;


using namespace std;

template<typename T>
static T lerp(const T a, const T b, float t)
{
    return a * (1.0f - t) + (b * t);
}

/// @class vec2
/// @brief Represents a 2D vector with x and y components and provides various operations.
class vec2 {
public:
    float x, y; ///< The x and y components of the vector.

    /// @brief Default constructor that initializes the vector to (0, 0).
    vec2() : x(0), y(0) {}

    /// @brief Constructor that initializes the vector with specified x and y components.
    /// @param _x The x component of the vector.
    /// @param _y The y component of the vector.
    vec2(float _x, float _y) : x(_x), y(_y) {}

    /// @brief Sets the position of the vector.
    /// @param _x The new x component.
    /// @param _y The new y component.
    void setPos(float _x, float _y) {
        x = _x;
        y = _y;
    }

    /// @brief Gets the current position of the vector.
    /// @return A new vec2 object representing the current position.
    vec2 getPos() { return vec2(x, y); }

    /// @brief Calculates the length (magnitude) of the vector.
    /// @return The length of the vector.
    float getLength() {
        return std::sqrt(x * x + y * y); // Corrected the length calculation formula.
    }

    /// @brief Adds two vectors and returns the result.
    /// @param v The vector to add.
    /// @return A new vec2 object that is the sum of the two vectors.
    vec2 operator+(const vec2& v) const {
        return vec2(x + v.x, y + v.y);
    }

    /// @brief Subtracts a vector from the current vector and returns the result.
    /// @param v The vector to subtract.
    /// @return A new vec2 object that is the difference of the two vectors.
    vec2 operator-(const vec2& v) const {
        return vec2(x - v.x, y - v.y);
    }

    /// @brief Subtracts a scalar from both components of the vector and returns the result.
    /// @param v The scalar value to subtract.
    /// @return A new vec2 object with the result.
    vec2 operator-(const float& v) const {
        return vec2(x - v, y - v);
    }

    /// @brief Adds a scalar to both components of the vector and returns the result.
    /// @param v The scalar value to add.
    /// @return A new vec2 object with the result.
    vec2 operator+(const float& v) const {
        return vec2(x + v, y + v);
    }

    /// @brief Divides both components of the vector by a scalar and returns the result.
    /// @param value The scalar value to divide by.
    /// @return A new vec2 object with the result, or the original vector if value is zero.
    vec2 operator/(const float value) const {
        if (value != 0) {
            return vec2(x / value, y / value);
        }
        else {
            std::cerr << "Attempt to divide by zero." << std::endl;
            return *this; // Return the original vector if divide by zero
        }
    }

    /// @brief Divides both components of the vector by a scalar and updates the vector.
    /// @param value The scalar value to divide by.
    void operator/=(const float value) {
        if (value != 0) {
            x /= value;
            y /= value;
        }
        else {
            std::cerr << "Attempt to divide by zero." << std::endl;
        }
    }

    /// @brief Multiplies both components of the vector by a scalar and returns the result.
    /// @param value The scalar value to multiply by.
    /// @return A new vec2 object with the result.
    vec2 operator*(const float value) const {
        return vec2(x * value, y * value);
    }

    /// @brief Adds another vector to the current vector and updates it.
    /// @param v The vector to add.
    void operator+=(const vec2& v) {
        x += v.x;
        y += v.y;
    }

    /// @brief Subtracts another vector from the current vector and updates it.
    /// @param v The vector to subtract.
    void operator-=(const vec2& v) {
        x -= v.x;
        y -= v.y;
    }
};

class vec3 {
public:
    vec3() : x(0), y(0), z(0)
    {}
    vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z)
    {}

    union {
        float v[3];
        struct { float x, y, z; };
        };

    float dot(const vec3& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
    }

    vec3 cross(const vec3& v) const {
        return vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Max and Min methods
    vec3 Max(const vec3& v1, const vec3& v2)
    {
        return vec3(max(v1.v[0], v2.v[0]),
                    max(v1.v[1], v2.v[1]),
                    max(v1.v[2], v2.v[2]));
    }

    float Max() const
    {
        return max(x, max(y, z));
    }


    vec3 Min(const vec3& v1, const vec3& v2)
    {
        return vec3(min(v1.v[0], v2.v[0]),
                    min(v1.v[1], v2.v[1]),
                    min(v1.v[2], v2.v[2]));
    }

    // Normalization methods
     vec3 normalize() const{
        float lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        if (lenSq == 0.0f) return *this;  // Return unchanged if the vector is zero-length
        float len = 1.0f / sqrtf(lenSq);
        return vec3(x * len, y * len, z * len);
    }

    float normalize_GetLength() {
        float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (length == 0.0f) return 0.0f;  // Return zero if the vector is zero-length
        float len = 1.0f / length;
        v[0] *= len; v[1] *= len; v[2] *= len;
        return length;
    }

    float lengthSq() const {
        return (SQ(v[0]) + SQ(v[1]) + SQ(v[2]));
    }

    // Vector-Vector Operators
    vec3 operator+(const vec3& pVec) const {
        return vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
    }

    vec3 operator-(const vec3& pVec) const {
        return vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
    }

    vec3 operator*(const vec3& pVec) const {
        return vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
    }

    vec3 operator/(const vec3& pVec) const {
        return vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
    }

    vec3& operator+=(const vec3& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        return *this;
    }

    vec3& operator-=(const vec3& pVec) {
        v[0] -= pVec.v[0];
        v[1] -= pVec.v[1];
        v[2] -= pVec.v[2];
        return *this;
    }

    // Vector-Scalar Operators
    vec3 operator+(float val) const {
        return vec3(v[0] + val, v[1] + val, v[2] + val);
    }

    vec3 operator-(float val) const {
        return vec3(v[0] - val, v[1] - val, v[2] - val);
    }

    vec3 operator*(float val) const {
        return vec3(v[0] * val, v[1] * val, v[2] * val);
    }

    vec3 operator/(float val) const {
        if (val == 0.0f) throw std::runtime_error("Division by zero");
        return vec3(v[0] / val, v[1] / val, v[2] / val);
    }

    vec3& operator+=(float val) {
        v[0] += val;
        v[1] += val;
        v[2] += val;
        return *this;
    }

    vec3& operator-=(float val) {
        v[0] -= val;
        v[1] -= val;
        v[2] -= val;
        return *this;
    }

    vec3& operator*=(float val) {
        v[0] *= val;
        v[1] *= val;
        v[2] *= val;
        return *this;
    }

    vec3& operator/=(float val) {
        if (val == 0.0f) throw std::runtime_error("Division by zero");
        v[0] /= val;
        v[1] /= val;
        v[2] /= val;
        return *this;
    }

    // Negation
    vec3 operator-() const {
        return vec3(-v[0], -v[1], -v[2]);
    }

    void print() const {
        std::cout << "vec4(" << x << ", " << y << ", " << z << ", " << ")\n";
    }
};

class vec4 {
public:
    vec4() : x(0), y(0), z(0), w(0)
    {}
    vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
    {}

    union {
        float v[4];
        struct { float x, y, z, w; };
        vec3 xyz = vec3(x, y, z);
    };

    float dot(const vec4& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
    }

    // Max and Min methods
    vec4 Max(const vec4& v1, const vec4& v2)
    {
        return vec4(max(v1.v[0], v2.v[0]),
                    max(v1.v[1], v2.v[1]),
                    max(v1.v[2], v2.v[2]),
                    max(v1.v[3], v2.v[3]));
    }

    vec4 Min(const vec4& v1, const vec4& v2)
    {
        return vec4(min(v1.v[0], v2.v[0]),
                    min(v1.v[1], v2.v[1]),
                    min(v1.v[2], v2.v[2]),
                    min(v1.v[3], v2.v[3]));
    }

    // Normalization methods
    vec4 normalize() {
        float lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
        if (lenSq == 0.0f) return *this;  // Return unchanged if the vector is zero-length
        float len = 1.0f / sqrtf(lenSq);
        return vec4(x * len, y * len, z * len, w * len);
    }

    float normalize_GetLength() {
        float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
        if (length == 0.0f) return 0.0f;  // Return zero if the vector is zero-length
        float len = 1.0f / length;
        v[0] *= len; v[1] *= len; v[2] *= len; v[3] *= len;
        return length;
    }

    float lengthSq() const {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
    }

    // Vector-Vector Operators
    vec4 operator+(const vec4& pVec) const {
        return vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
    }

    vec4 operator-(const vec4& pVec) const {
        return vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
    }

    vec4 operator*(const vec4& pVec) const {
        return vec4(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2], v[3] * pVec.v[3]);
    }

    vec4 operator/(const vec4& pVec) const {
        return vec4(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2], v[3] / pVec.v[3]);
    }

    vec4& operator+=(const vec4& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        v[3] += pVec.v[3];
        return *this;
    }

    vec4& operator-=(const vec4& pVec) {
        v[0] -= pVec.v[0];
        v[1] -= pVec.v[1];
        v[2] -= pVec.v[2];
        v[3] -= pVec.v[3];
        return *this;
    }

    // Vector-Scalar Operators
    vec4 operator+(float val) const {
        return vec4(v[0] + val, v[1] + val, v[2] + val, v[3] + val);
    }

    vec4 operator-(float val) const {
        return vec4(v[0] - val, v[1] - val, v[2] - val, v[3] - val);
    }

    vec4 operator*(float val) const {
        return vec4(v[0] * val, v[1] * val, v[2] * val, v[3] * val);
    }

    vec4 operator/(float val) const {
        if (val == 0.0f) throw std::runtime_error("Division by zero");
        return vec4(v[0] / val, v[1] / val, v[2] / val, v[3] / val);
    }

    vec4& operator+=(float val) {
        v[0] += val;
        v[1] += val;
        v[2] += val;
        v[3] += val;
        return *this;
    }

    vec4& operator-=(float val) {
        v[0] -= val;
        v[1] -= val;
        v[2] -= val;
        v[3] -= val;
        return *this;
    }

    vec4& operator*=(float val) {
        v[0] *= val;
        v[1] *= val;
        v[2] *= val;
        v[3] *= val;
        return *this;
    }

    vec4& operator/=(float val) {
        if (val == 0.0f) throw std::runtime_error("Division by zero");
        v[0] /= val;
        v[1] /= val;
        v[2] /= val;
        v[3] /= val;
        return *this;
    }

    // Negation
    vec4 operator-() const {
        return vec4(-v[0], -v[1], -v[2], -v[3]);
    }

    void print() const {
        std::cout << "vec4(" << x << ", " << y << ", " << z << ", " << w << ")\n";
    }
};

class Matrix {
public:
    union {
        float a[4][4];
        float m[16];
    };

    Matrix() { identity(); }

    Matrix(float* otherMatrix) {
        memcpy(&m, otherMatrix, 16 * sizeof(float));
    }

    Matrix(float m00, float m01, float m02, float m03, 
           float m10, float m11, float m12, float m13, 
           float m20, float m21, float m22, float m23, 
           float m30, float m31, float m32, float m33) {  

        a[0][0] = m00;  a[0][1] = m01;  a[0][2] = m02;  a[0][3] = m03;
        a[1][0] = m10;  a[1][1] = m11;  a[1][2] = m12;  a[1][3] = m13;
        a[2][0] = m20;  a[2][1] = m21;  a[2][2] = m22;  a[2][3] = m23;
        a[3][0] = m30;  a[3][1] = m31;  a[3][2] = m32;  a[3][3] = m33;
    }

    void identity() {
        memset(m, 0, 16 * sizeof(float));
        m[0] = 1.f;
        m[5] = 1.f;
        m[10] = 1.f;
        m[15] = 1.f;
    }

    vec3 mulPoint(const vec3& v) const
    {
        vec3 v1 = vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
            (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
            (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
        float w;
        w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
        w = 1.0f / w;
        return (v1 * w);
    }

    vec3 mulVec(const vec3& v)
    {
        return vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]),
            (v.x * m[4] + v.y * m[5] + v.z * m[6]),
            (v.x * m[8] + v.y * m[9] + v.z * m[10]));
    }

    Matrix mul(const Matrix& matrix) const
    {
        Matrix ret;
        ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
        ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
        ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
        ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];

        ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
        ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
        ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
        ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];

        ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
        ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
        ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
        ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];

        ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
        ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
        ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
        ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
        return ret;
    }

    Matrix invert()
    {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0) {
            // Handle this case
        }
        det = 1.0 / det;
        for (int i = 0; i < 16; i++) {
            inv[i] = inv[i] * det;
        }
        return inv;
    }

    Matrix operator*(const Matrix& matrix)
    {
        return mul(matrix);
    }

    float& operator[](int index) {
        return m[index];
    }

    static Matrix translation(const vec3& v) {
        Matrix mat;
        mat.a[0][3] = v.x;
        mat.a[1][3] = v.y;
        mat.a[2][3] = v.z;
        return mat;
    }

    static Matrix scaling(const vec3& v) {
        Matrix mat;
        mat.m[0] = v.x;
        mat.m[5] = v.y;
        mat.m[10] = v.z;
        return mat;
    }

    static Matrix rotateX(float theta) {
        Matrix mat;
        float ct = cosf(theta);
        float st = sinf(theta);
        mat.m[5] = ct;
    }

    static Matrix Projection(float _fov, float _aspect, float _near, float _far)
    {
        float dFov = _fov * M_PI / 180; // contains conversion from degree
        float tanHalfFov = tan(_fov / 2);

        Matrix _mat;

        _mat.m[0] = 1.0f / (_aspect * tanHalfFov);
        _mat.m[5] = 1.0f / tanHalfFov;
        _mat.m[10] = -_far / (_far - _near);

        _mat.m[11] = -(_far * _near) / (_far - _near);
        _mat.m[14] = -1.0f;
        _mat.m[15] = 0.0f;

        return _mat;
    }

    static Matrix LookAt(vec3 _from, vec3 _to, vec3 _up)
    {
        vec3 forward = (_from - _to).normalize();
        vec3 right =_up.cross(forward);
        vec3 up = forward.cross(right);

        Matrix mat;

        mat.a[0][0] = right.x;
        mat.a[0][1] = right.y;
        mat.a[0][2] = right.z;

        mat.a[1][0] = up.x;
        mat.a[1][1] = up.y;
        mat.a[1][2] = up.z;

        mat.a[2][0] = forward.x;
        mat.a[2][1] = forward.y;
        mat.a[2][2] = forward.z;

        mat.a[0][3] = -_from.dot(right);
        mat.a[1][3] = -_from.dot(up);
        mat.a[2][3] = -_from.dot(forward);

        return mat;
    }

    static Matrix View(vec3 _pos, vec3 _forward)
    {
        _forward = _forward.normalize();
        vec3 right = vec3(0, 1, 0).cross(_forward);
        vec3 up = _forward.cross(right);

        Matrix mat;

        mat.a[0][0] = right.x;
        mat.a[1][0] = right.y;
        mat.a[2][0] = right.z;

        mat.a[0][1] = up.x;
        mat.a[1][1] = up.y;
        mat.a[2][1] = up.z;

        mat.a[0][2] = -_forward.x;
        mat.a[1][2] = -_forward.y;
        mat.a[2][2] = -_forward.z;

        mat.a[0][3] = -(_pos.dot(right));
        mat.a[1][3] = -(_pos.dot(up));
        mat.a[2][3] = _pos.dot(_forward);

        return mat;
    }

};

class Quaternion {
public:
    // Constructors
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

    union {
        float q[4];
        struct { float w, x, y, z; };
    };

    // Quaternion multiplication
    Quaternion operator*(const Quaternion& q2) const {
        return Quaternion(
            q[0] * q2.q[0] - q[1] * q2.q[1] - q[2] * q2.q[2] - q[3] * q2.q[3],
            q[0] * q2.q[1] + q[1] * q2.q[0] + q[2] * q2.q[3] - q[3] * q2.q[2],
            q[0] * q2.q[2] - q[1] * q2.q[3] + q[2] * q2.q[0] + q[3] * q2.q[1],
            q[0] * q2.q[3] + q[1] * q2.q[2] - q[2] * q2.q[1] + q[3] * q2.q[0]
        );
    }

    // Quaternion addition
    Quaternion operator+(const Quaternion& q2) const {
        return Quaternion(q[0] + q2.q[0], q[1] + q2.q[1], q[2] + q2.q[2], q[3] + q2.q[3]);
    }

    // Normalization
    Quaternion normalize() const {
        float len = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
        if (len == 0.0f) return *this;
        float invLen = 1.0f / len;
        return Quaternion(q[0] * invLen, q[1] * invLen, q[2] * invLen, q[3] * invLen);
    }

    // Slerp (Spherical Linear Interpolation)
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float dot = q1.q[0] * q2.q[0] + q1.q[1] * q2.q[1] + q1.q[2] * q2.q[2] + q1.q[3] * q2.q[3];
        Quaternion q2b = q2;

        // If the dot product is negative, slerp won't take the shorter path.
        if (dot < 0.0f) {
            dot = -dot;
            q2b = Quaternion(-q2.q[0], -q2.q[1], -q2.q[2], -q2.q[3]);
        }

        const float THRESHOLD = 0.9995f;
        if (dot > THRESHOLD) {
            // Linear interpolation when the quaternions are close
            Quaternion result = q1 + (q2b - q1) * t;
            return result.normalize();
        }

        float theta_0 = acosf(dot);
        float theta = theta_0 * t;
        float sin_theta = sinf(theta);
        float sin_theta_0 = sinf(theta_0);

        float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        return (q1 * s0) + (q2b * s1);
    }

    // Convert to a rotation matrix
    Matrix toMatrix() const {
        Matrix mat;
        mat.identity();

        float xx = q[1] * q[1], yy = q[2] * q[2], zz = q[3] * q[3];
        float xy = q[1] * q[2], xz = q[1] * q[3], yz = q[2] * q[3];
        float wx = q[0] * q[1], wy = q[0] * q[2], wz = q[0] * q[3];

        mat.m[0] = 1.0f - 2.0f * (yy + zz);
        mat.m[1] = 2.0f * (xy - wz);
        mat.m[2] = 2.0f * (xz + wy);

        mat.m[4] = 2.0f * (xy + wz);
        mat.m[5] = 1.0f - 2.0f * (xx + zz);
        mat.m[6] = 2.0f * (yz - wx);

        mat.m[8] = 2.0f * (xz - wy);
        mat.m[9] = 2.0f * (yz + wx);
        mat.m[10] = 1.0f - 2.0f * (xx + yy);

        return mat;
    }

    // Negation
    Quaternion operator-() const {
        return Quaternion(-w, -x, -y, -z);
    }

    // Scalar multiplication
    Quaternion operator*(float val) const {
        return Quaternion(w * val, x * val, y * val, z * val);
    }

    // Quaternion subtraction
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }

    // Print function for debugging
    void print() const {
        std::cout << "Quaternion(" << w << ", " << x << ", " << y << ", " << z << ")\n";
    }
};

class Colour {
public:
    // Constructors
    Colour() : r(0), g(0), b(0) {}

    Colour(float _r, float _g, float _b)
        : r(_r), g(_g), b(_b) {}

    Colour(unsigned char _r, unsigned char _g, unsigned char _b)
        : r(_r / 255.0f), g(_g / 255.0f), b(_b / 255.0f) {}

    // Member variables
    union {
        float v[3];
        struct { float r, g, b; };
        vec3 rgb = vec3(r, g, b);
    };

    // Methods for adding, subtracting, and multiplying colors
    Colour operator+(const Colour& c) const {
        return Colour(r + c.r, g + c.g, b + c.b);
    }

    Colour operator-(const Colour& c) const {
        return Colour(r - c.r, g - c.g, b - c.b);
    }

    Colour operator*(float val) const {
        return Colour(r * val, g * val, b * val);
    }

    Colour& operator+=(const Colour& c) {
        r = r + c.r;
        g = g + c.g;
        b = b + c.b;
        return *this;
    }

    Colour& operator-=(const Colour& c) {
        r = r - c.r;
        g = g - c.g;
        b = b - c.b;
        return *this;
    }

    Colour& operator*=(float val) {
        r = r * val;
        g = g * val;
        b = b * val;
        return *this;
    }

    // Method to print the color for debugging
    void print() const {
        std::cout << "Colour(" << r << ", " << g << ", " << b << ")\n";
    }
};

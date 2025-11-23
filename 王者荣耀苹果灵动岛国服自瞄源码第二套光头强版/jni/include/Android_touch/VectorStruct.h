/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <math.h>

#ifndef GAMEHELPER_VECTORSTRUCT_H
#define GAMEHELPER_VECTORSTRUCT_H

struct Vector2 {
    float x;
    float y;

    inline Vector2() : x(0), y(0) {};

    inline Vector2(const float x, const float y) : x(x), y(y) {};


    inline Vector2 operator+(const Vector2 &other) const {
        return Vector2(x + other.x, y + other.y);
    }

    inline Vector2 operator+(const float other) const {
        return Vector2(x + other, y + other);
    }

    inline Vector2 operator-(const Vector2 &other) const {
        return Vector2(x - other.x, y - other.y);
    }

    inline Vector2 operator-(const float other) const {
        return Vector2(x - other, y - other);
    }

    inline Vector2 operator*(const Vector2 &other) const {
        return Vector2(x * other.x, y * other.y);
    }

    inline Vector2 operator*(const float value) const {
        return Vector2(x * value, y * value);
    }

    inline Vector2 operator/(const Vector2 &other) const {
        if (other.x != 0 && other.y != 0) {
            return Vector2(x / other.x, y / other.y);
        }
        return Vector2();
    }

    inline Vector2 operator/(const float value) const {
        if (value != 0) {
            return Vector2(x / value, y / value);
        }
        return Vector2();
    }

    inline Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    inline Vector2 &operator+=(const Vector2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }


    inline Vector2 &operator-=(const Vector2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Vector2 &operator+=(const float value) {
        x += value;
        y += value;
        return *this;
    }

    inline Vector2 &operator-=(const float value) {
        x -= value;
        y -= value;
        return *this;
    }

    inline Vector2 &operator*=(const float value) {
        x *= value;
        y *= value;
        return *this;
    }

    inline Vector2 &operator*=(const Vector2 &other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline Vector2 &operator/=(const float &value) {
        x /= value;
        y /= value;
        return *this;
    }

    inline Vector2 &operator=(const Vector2 &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    inline bool operator==(const Vector2 &other) const {
        return x == other.x && y == other.y;
    }

    inline bool operator!=(const Vector2 &other) const {
        return x != other.x || y != other.y;
    }

    inline float operator[](int index) const {
        return (&x)[index];
    }

    inline float &operator[](int index) {
        return (&x)[index];
    }

    inline bool NotHaveZero() {
        return x != 0 && y != 0;
    }

    inline void zero() {
        x = y = 0;
    }

    inline float length() const {
        return sqrt(x * x + y * y);
    }
    
    inline float Length() const {
    return sqrt(x * x + y * y);
  }

    inline Vector2 Normalize() const {
    float len = length();
    if (len > 0) {
      return Vector2(x / len, y / len);
    }
    return Vector2(0, 0);
  }

};

#ifndef VECTOR3_DEFINED
#define VECTOR3_DEFINED
struct Vector3 {
    float x;
    float y;
    float z;

    inline Vector3() : x(0), y(0), z(0) {}

    inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    inline  explicit Vector3(float value) : x(value), y(value), z(value) {}

    inline Vector3 operator+(const Vector3 &other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    inline Vector3 operator+(const float other) const {
        return Vector3(x + other, y + other, z + other);
    }

    inline Vector3 operator-(const Vector3 &other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    inline Vector3 operator-(const float other) const {
        return Vector3(x - other, y - other, z - other);
    }

    inline Vector3 operator*(const Vector3 &other) const {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    inline Vector3 operator*(const float value) const {
        return Vector3(x * value, y * value, z * value);
    }

    inline Vector3 operator/(const float value) const {
        if (value != 0) {
            return Vector3(x / value, y / value, z / value);
        }
        return Vector3();
    }

    inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    inline Vector3 &operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline Vector3 &operator-=(const Vector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline Vector3 &operator+=(const float value) {
        x += value;
        y += value;
        z += value;
        return *this;
    }

    inline Vector3 &operator-=(const float value) {
        x -= value;
        y -= value;
        z -= value;
        return *this;
    }

    inline Vector3 &operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    inline Vector3 &operator*=(const Vector3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    inline Vector3 &operator/=(const float &value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    inline Vector3 &operator=(const Vector3 &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    inline bool operator==(const Vector3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    inline bool operator!=(const Vector3 &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    inline float operator[](int index) const {
        return (&x)[index];
    }

    inline float &operator[](int index) {
        return (&x)[index];
    }

    inline void Zero() {
        x = y = z = 0;
    }

    inline float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    inline bool isValid() const {
        return x != 0 && y != 0 && z != 0 && !isnan(x) && !isnan(y) && !isnan(z);
    }

    static inline float dot(const Vector3 &a, const Vector3 &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static inline bool inRange(const Vector3 &target, const Vector3 &min, const Vector3 &max) {
        return target.x > min.x && target.x < max.x && target.y > min.y && target.y < max.y &&
               target.z > min.z && target.z < max.z;
    }
};

// Unity3D 格式的 Vector3 (XZY 顺序)
struct Vector3Unity {
    float X;
    float Z; 
    float Y;
    
    inline Vector3Unity() : X(0), Z(0), Y(0) {}
    inline Vector3Unity(float x, float z, float y) : X(x), Z(z), Y(y) {}
    
    // 转换函数
    inline Vector3 toVector3() const { return Vector3(X, Y, Z); }
    inline static Vector3Unity fromVector3(const Vector3& v) { return Vector3Unity(v.x, v.z, v.y); }
};

#endif // VECTOR3_DEFINED

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    inline Vector4() : x(0), y(0), z(0), w(0) {}

    inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    inline explicit Vector4(float value) : x(value), y(value), z(value), w(value) {}

    inline Vector4 operator+(const Vector4 &other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    inline Vector4 operator+(const float other) const {
        return Vector4(x + other, y + other, z + other, w + other);
    }

    inline Vector4 operator-(const Vector4 &other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    inline Vector4 operator-(const float other) const {
        return Vector4(x - other, y - other, z - other, w - other);
    }

    inline Vector4 operator*(const float value) const {
        return Vector4(x * value, y * value, z * value, w * value);
    }

    inline Vector4 operator*(const Vector4 &other) const {
        return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    inline Vector4 operator/(const float value) const {
        if (value != 0) {
            return Vector4(x / value, y / value, z / value, w / value);
        }
        return Vector4();
    }


    inline Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }

    inline Vector4 &operator+=(const Vector4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    inline Vector4 &operator-=(const Vector4 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    inline Vector4 &operator+=(const float value) {
        x += value;
        y += value;
        z += value;
        w += value;
        return *this;
    }

    inline Vector4 &operator-=(const float value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;
        return *this;
    }

    inline Vector4 &operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
        return *this;
    }

    inline Vector4 &operator*=(const Vector4 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    inline Vector4 &operator/=(const float value) {
        x /= value;
        y /= value;
        z /= value;
        w /= value;
        return *this;
    }

    inline Vector4 &operator=(const Vector4 &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    inline bool operator==(const Vector4 &other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    inline bool operator!=(const Vector4 &other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    inline float operator[](int index) const {
        return (&x)[index];
    }

    inline float &operator[](int index) {
        return (&x)[index];
    }

    inline void Zero() {
        x = y = z = w = 0;
    }

    inline bool NotHaveZero() {
        return x != 0 && y != 0 && z != 0 && w != 0;
    }



};

inline Vector4 vec4_mult(const Vector4 &vec, const Vector4 &vec2) {
    return vec * vec2;
}

inline Vector4 vec4_piu(const Vector4 &vec, const Vector4 &vec2) {
    return vec + vec2;
}

inline Vector4 vec4_meno(const Vector4 &vec, const Vector4 &vec2) {
    return vec - vec2;
}

inline float q2djl(const Vector2 &vec, const Vector2 &vec2) {
    return (vec - vec2).length();
}

inline float q3djl(const Vector3 &vec, const Vector3 &vec2) {
    return (vec - vec2).length();
}

inline bool isInRange(const Vector2 &target, const Vector2 &min, const Vector2 &max) {
    return target.x > min.x && target.x < max.x && target.y > min.y && target.y < max.y;
}

inline bool isInRange(const Vector3 &target, const Vector3 &min, const Vector3 &max) {
    return target.x > min.x && target.x < max.x && target.y > min.y && target.y < max.y && target.z > min.z &&
           target.z < max.z;
}

struct _Int2 {
    int x;
    int y;
};

struct _Int3 {
    int x;
    int y;
    int z;
};


#endif

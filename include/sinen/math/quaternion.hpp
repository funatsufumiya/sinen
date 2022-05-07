#pragma once
#include "math.hpp"
#include "vector3.hpp"
namespace nen {
// (Unit) Quaternion
class quaternion {
public:
  float x{};
  float y{};
  float z{};
  float w{};

  quaternion() { *this = quaternion::Identity; }

  explicit quaternion(float inX, float inY, float inZ, float inW) {
    Set(inX, inY, inZ, inW);
  }

  explicit quaternion(const vector3 &axis, float angle) {
    const auto scalar = math::Sin(angle / 2.0f);
    x = axis.x * scalar;
    y = axis.y * scalar;
    z = axis.z * scalar;
    w = math::Cos(angle / 2.0f);
  }

  // Directly set the internal components
  void Set(float inX, float inY, float inZ, float inW) {
    x = inX;
    y = inY;
    z = inZ;
    w = inW;
  }

  void Conjugate() {
    x *= -1.0f;
    y *= -1.0f;
    z *= -1.0f;
  }

  [[nodiscard]] float LengthSq() const {
    return (x * x + y * y + z * z + w * w);
  }

  [[nodiscard]] float Length() const { return math::Sqrt(LengthSq()); }

  void Normalize() {
    const auto length = Length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
  }

  // Normalize the provided quaternion
  static quaternion Normalize(const quaternion &q) {
    auto retVal = q;
    retVal.Normalize();
    return retVal;
  }

  // Linear interpolation
  static quaternion Lerp(const quaternion &a, const quaternion &b, float f) {
    quaternion retVal;
    retVal.x = math::Lerp(a.x, b.x, f);
    retVal.y = math::Lerp(a.y, b.y, f);
    retVal.z = math::Lerp(a.z, b.z, f);
    retVal.w = math::Lerp(a.w, b.w, f);
    retVal.Normalize();
    return retVal;
  }

  static float Dot(const quaternion &a, const quaternion &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }

  // Spherical Linear Interpolation
  static quaternion Slerp(const quaternion &a, const quaternion &b, float f) {
    const auto rawCosm = quaternion::Dot(a, b);

    auto cosom = -rawCosm;
    if (rawCosm >= 0.0f) {
      cosom = rawCosm;
    }

    float scale0, scale1;

    if (cosom < 0.9999f) {
      const auto omega = math::Acos(cosom);
      const auto invSin = 1.f / math::Sin(omega);
      scale0 = math::Sin((1.f - f) * omega) * invSin;
      scale1 = math::Sin(f * omega) * invSin;
    } else {
      scale0 = 1.0f - f;
      scale1 = f;
    }

    if (rawCosm < 0.0f) {
      scale1 = -scale1;
    }

    quaternion retVal;
    retVal.x = scale0 * a.x + scale1 * b.x;
    retVal.y = scale0 * a.y + scale1 * b.y;
    retVal.z = scale0 * a.z + scale1 * b.z;
    retVal.w = scale0 * a.w + scale1 * b.w;
    retVal.Normalize();
    return retVal;
  }

  // Concatenate
  // Rotate by q FOLLOWED BY p
  static quaternion Concatenate(const quaternion &q, const quaternion &p) {
    quaternion retVal;

    const vector3 qv(q.x, q.y, q.z);
    const vector3 pv(p.x, p.y, p.z);
    const auto newVec = p.w * qv + q.w * pv + vector3::Cross(pv, qv);
    retVal.x = newVec.x;
    retVal.y = newVec.y;
    retVal.z = newVec.z;

    retVal.w = p.w * q.w - vector3::Dot(pv, qv);

    return retVal;
  }

  static vector3 ToEuler(const quaternion &r);

  static const quaternion Identity;
};
} // namespace nen
#ifndef PARTICLE_H
#define PARTICLE_H
#include <cmath>
#include <stdint.h>

template <typename VectorType>
class MechanicalObject {
public:
  void setPosition(const VectorType &position) {
    m_position = position;
  }

  void setVelocity(const VectorType &velocity) {
    m_velocity = velocity;
  }

  void applayForce(double deltaTime, const VectorType &acceleration) {
    m_velocity += (acceleration * deltaTime);
  }

  void proceed(double deltaTime) {
    m_position += (m_velocity * deltaTime);
  }

  VectorType getPosition() const {
    return m_position;
  }

  VectorType getVelocity() const {
    return m_velocity;
  }


  virtual ~MechanicalObject() {}

protected:
  VectorType m_position;
  VectorType m_velocity;
};

class ObjectWithLifeTime {
public:
  ObjectWithLifeTime() : m_startTime_ms(0), m_lifeDuration_ms(0) {}
  bool isExpired(int64_t time_ms) const;
  void setLifeTime(int64_t startTime_ms, int64_t lifeDuration_ms);
  int64_t getLifeTimeDur() const;
  virtual ~ObjectWithLifeTime();

private:
  int64_t m_startTime_ms;
  int64_t m_lifeDuration_ms;
};

class Drawable {
public:
  virtual void draw() = 0;
  virtual ~Drawable();
};

template<typename VectorType>
class AbstractParticle
    : public MechanicalObject<VectorType>
    , public ObjectWithLifeTime
    , public Drawable {};

struct Vec2d {
  Vec2d() : x (0), y(0) {}
  Vec2d(double x, double y): x(x), y(y) {}

  Vec2d &operator+=(const Vec2d& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vec2d &operator*=(int rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Vec2d operator+(const Vec2d& rhs) const {
    return Vec2d(x + rhs.x, y + rhs.y);
  }

  Vec2d operator*(double rhs) const {
    return Vec2d(x * rhs, y * rhs);
  }

  Vec2d rotated(double angle) const {
    angle *= (M_PI / 180); // to radians
    double cs = std::cos(angle);
    double sn = std::sin(angle);
    return Vec2d(x * cs - y * sn, x * sn + y * cs);
  }

  double x, y;
};

#endif // PARTICLE_H

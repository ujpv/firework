#ifndef FIREWORKS_2D_OPENGL_H
#define FIREWORKS_2D_OPENGL_H
#include <list>
#include <memory>
#include <chrono>

#include "particle.h"

class Fireworks_2D_OpenGL {
private:
  class FireworksPartition;
  class BigRocket;
  class SmallRocket;
  class Spark;

public:
  Fireworks_2D_OpenGL()
    : m_lastCallTime_ms(timeNow())
    , m_nextSartTime_ms(0)
    , m_gravity(0, -GRAVITY)
  {}
  typedef std::list<std::unique_ptr<FireworksPartition>> PollType;
  void visitFinalize(const BigRocket&   particle);
  void visitFinalize(const SmallRocket& particle);
  void draw();
  void proceed();

private:

  class FireworksPartition
      : public AbstractParticle<Vec2d> {
  public:
    FireworksPartition(PollType &homePoll);
    PollType &getHomePoll();
    virtual void finalize(Fireworks_2D_OpenGL &fireWork) = 0;
    virtual ~FireworksPartition();
  private:
    PollType &m_homePoll;
  };

  class BigRocket : public FireworksPartition {
  public:
    BigRocket(PollType &homePoll) : FireworksPartition(homePoll) {}
    void draw() override;
    void finalize(Fireworks_2D_OpenGL &fireWork) override { fireWork.visitFinalize(*this); }
  };

  class SmallRocket : public FireworksPartition {
  public:
    SmallRocket(PollType &homePoll) : FireworksPartition(homePoll) {}
    void draw() override;
    void finalize(Fireworks_2D_OpenGL &fireWork) override { fireWork.visitFinalize(*this); }
  };

  class Spark : public FireworksPartition {
  public:
    Spark(PollType &homePoll) : FireworksPartition(homePoll) {}
    void draw() override;
    void finalize(Fireworks_2D_OpenGL &) override {}
  };

  static int64_t timeNow();
  FireworksPartition &activateParticale(PollType &poll);
  void createParticales();

  PollType m_activePartitions;
  PollType m_bigRocketPartitions;
  PollType m_smallRocketPartitions;
  PollType m_sparkPartitions;

  int64_t m_lastCallTime_ms;
  int64_t m_nextSartTime_ms;

  Vec2d m_gravity;

  static constexpr double GRAVITY = 2;
  static constexpr double START_VELOCITY = 2.2;
  static constexpr int ANGLE_DISPERTION = 30;
  static constexpr int RESTART_PERIOD_MS = 500;
  static constexpr int MIN_LIFE_TIME_MS = 300;
  static constexpr int MAX_LIFE_TIME_MS = 1000;
  static constexpr int MIN_SPARK_COUNT = 50;
  static constexpr int MAX_SPARK_COUNT = 80;
  static constexpr double LIFE_TIME_TO_VEL_CONST = 1000;
};

#endif // FIREWORKS_2D_OPENGL_H

#include <iostream>
#include <GL/glut.h>
#include "fireworks_2d_opengl.h"

void Fireworks_2D_OpenGL::BigRocket::draw() {
  glPushMatrix();
  glTranslated(m_position.x, m_position.y, 0.0);
  glColor3d(0, 0, 1.0);
  glutSolidSphere(0.01, 5, 2);
  glPopMatrix();
}

void Fireworks_2D_OpenGL::SmallRocket::draw() {
  glPushMatrix();
  glTranslated(m_position.x, m_position.y, 0.0);
  glColor3d(1, 1, 0);
  glutSolidSphere(0.005, 5, 2);
  glPopMatrix();
}

void Fireworks_2D_OpenGL::Spark::draw() {
  glPushMatrix();
  glTranslated(m_position.x, m_position.y, 0.0);
  glColor3d(1, 1, 1);
  glutSolidSphere(0.001, 5, 2);
  glPopMatrix();
}

void Fireworks_2D_OpenGL::visitFinalize(const Fireworks_2D_OpenGL::BigRocket &particle) {
  int sparkCount = MIN_SPARK_COUNT + rand() % (MAX_SPARK_COUNT - MIN_SPARK_COUNT);
  int64_t timeNow_ms = timeNow();
  int64_t halfLifeTime = particle.getLifeTimeDur() / 2;
  Vec2d position = particle.getPosition();
  Vec2d velocity = particle.getVelocity();
  for (int i = 0; i < sparkCount; ++i) {
    FireworksPartition &newParticle = activateParticale(m_sparkPartitions);
    newParticle.setLifeTime(timeNow_ms, halfLifeTime + rand() % (halfLifeTime / 10));
    newParticle.setPosition(position);
    double newVelocity = halfLifeTime / LIFE_TIME_TO_VEL_CONST + (rand() % halfLifeTime / LIFE_TIME_TO_VEL_CONST / 2);
    newParticle.setVelocity(velocity + Vec2d(0, newVelocity).rotated(rand() % 360));
  }

  for (int i = 0; i < 2; ++i) {
    FireworksPartition &newParticle = activateParticale(m_smallRocketPartitions);
    newParticle.setLifeTime(timeNow_ms, halfLifeTime + rand() % (halfLifeTime));
    newParticle.setPosition(position);
    newParticle.setVelocity((velocity * 2).rotated(rand() % 30 + 15));
  }
}

void Fireworks_2D_OpenGL::visitFinalize(const Fireworks_2D_OpenGL::SmallRocket &particle) {
  int sparkCount = MIN_SPARK_COUNT + rand() % (MAX_SPARK_COUNT - MIN_SPARK_COUNT);
  int64_t timeNow_ms = timeNow();
  int64_t halfLifeTime = particle.getLifeTimeDur() / 2;
  Vec2d position = particle.getPosition();
  Vec2d velocity = particle.getVelocity();
  for (int i = 0; i < sparkCount; ++i) {
    FireworksPartition &newParticle = activateParticale(m_sparkPartitions);
    newParticle.setLifeTime(timeNow_ms, halfLifeTime + rand() % (halfLifeTime / 10));
    newParticle.setPosition(position);
    double newVelocity = halfLifeTime / LIFE_TIME_TO_VEL_CONST + (rand() % halfLifeTime / LIFE_TIME_TO_VEL_CONST / 2);
    newParticle.setVelocity(velocity + Vec2d(0, newVelocity).rotated(rand() % 360));
  }
}

void Fireworks_2D_OpenGL::draw() {
  for (auto &p: m_activePartitions)
    p->draw();
}

void Fireworks_2D_OpenGL::proceed() {
  int64_t timeNow_ms = timeNow();
  double delta_time_sec = (timeNow_ms - m_lastCallTime_ms) / 1000.0;

  if (timeNow_ms > m_nextSartTime_ms) {
    FireworksPartition &partition = activateParticale(m_bigRocketPartitions);
    partition.setPosition(Vec2d(0, -1));
    partition.setLifeTime(timeNow(), MIN_LIFE_TIME_MS + rand() % (MAX_LIFE_TIME_MS - MIN_LIFE_TIME_MS));
    partition.setVelocity(Vec2d(0, START_VELOCITY).rotated(rand() % ANGLE_DISPERTION - ANGLE_DISPERTION / 2));
    m_nextSartTime_ms = timeNow_ms + rand() % RESTART_PERIOD_MS;
  }

  for (auto it = m_activePartitions.begin(); it != m_activePartitions.end();) {
    FireworksPartition &particle = *(*it);
    particle.applayForce(delta_time_sec, m_gravity);
    particle.proceed(delta_time_sec);
    if(particle.isExpired(timeNow_ms)) {
      particle.finalize(*this);
      PollType &home = particle.getHomePoll();
      home.splice(home.begin(), m_activePartitions, it++);
    } else {
      ++it;
    }
  }

  m_lastCallTime_ms = timeNow_ms;
}

int64_t Fireworks_2D_OpenGL::timeNow() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

Fireworks_2D_OpenGL::FireworksPartition &Fireworks_2D_OpenGL::activateParticale(
    Fireworks_2D_OpenGL::PollType &poll) {
  if (poll.empty())
    createParticales();
  m_activePartitions.splice(m_activePartitions.end(), poll, poll.begin());
  return *m_activePartitions.back();
}

void Fireworks_2D_OpenGL::createParticales() {
  int count = 1000;
  if (m_bigRocketPartitions.empty()) {
    for (int i = 0; i < count; ++i) {
      m_bigRocketPartitions.push_back(
            std::unique_ptr<FireworksPartition>(
              new BigRocket(m_bigRocketPartitions)));
    }
  }

  if (m_smallRocketPartitions.empty()) {
    for (int i = 0; i < count; ++i) {
      m_smallRocketPartitions.push_back(
            std::unique_ptr<FireworksPartition>(
              new SmallRocket(m_smallRocketPartitions)));
    }
  }

  if (m_sparkPartitions.empty()) {
    for (int i = 0; i < count; ++i) {
      m_sparkPartitions.push_back(
            std::unique_ptr<FireworksPartition>(
              new Spark(m_sparkPartitions)));
    }
  }
}

Fireworks_2D_OpenGL::FireworksPartition::FireworksPartition(Fireworks_2D_OpenGL::PollType &homePoll)
  : m_homePoll(homePoll)
{}

Fireworks_2D_OpenGL::PollType &Fireworks_2D_OpenGL::FireworksPartition::getHomePoll() {
  return m_homePoll;
}

Fireworks_2D_OpenGL::FireworksPartition::~FireworksPartition() {

}

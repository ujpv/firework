#include "particle.h"

bool ObjectWithLifeTime::isExpired(int64_t time_ms) const {
  return time_ms - m_startTime_ms > m_lifeDuration_ms;
}

void ObjectWithLifeTime::setLifeTime(int64_t startTime_ms, int64_t lifeDuration_ms)
{
  m_startTime_ms = startTime_ms;
  m_lifeDuration_ms = lifeDuration_ms;
}

int64_t ObjectWithLifeTime::getLifeTimeDur() const {
  return m_lifeDuration_ms;
}

ObjectWithLifeTime::~ObjectWithLifeTime() {}

Drawable::~Drawable() {}


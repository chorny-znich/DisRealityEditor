#include "location.h"

void Location::setId(size_t id)
{
  mId = id;
}

size_t Location::getId() const
{
  return mId;
}

void Location::setPosition(sf::Vector2u pos)
{
  mPosition = pos;
}

sf::Vector2u Location::getPosition() const
{
  return mPosition;
}

void Location::setFloorLayerId(const std::string& id)
{
  mFloorLayerId = id;
}

const std::string& Location::getFloorLayerId() const
{
  return mFloorLayerId;
}

void Location::setObjectLayerId(const std::string& id)
{
  mObjectLayerId = id;
}

const std::string& Location::getObjectLayerId() const
{
  return mObjectLayerId;
}

void Location::setBarrier(bool value)
{
  mBarrier = value;
}

bool Location::isBarrier() const
{
  return mBarrier;
}

void Location::setPlayer(bool value)
{
  mPlayer = value;
}

bool Location::isPlayer() const
{
  return mPlayer;
}

void Location::setEnemy(bool value)
{
  mEnemy = value;
}

bool Location::isEnemy() const
{
    return mEnemy;
} 
void Location::setObject(bool value)
{
  mObject = value;
}
bool Location::isObject() const
{
  return mObject;
}

void Location::setNPC(bool value)
{
  mNpc = value;
}

bool Location::isNpc() const
{
    return mNpc;
}

#pragma once

#include <memory>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>

class Location
{
private:
  size_t mId;
  sf::Vector2u mPosition;
  std::string mFloorLayerId;
  std::string mObjectLayerId;
  bool mBarrier{ false };
  bool mPlayer{ false };
  bool mEnemy{ false };
  bool mObject{ false };
  bool mNpc{ false };

public:
  void setId(size_t id);
  size_t getId() const;
  void setPosition(sf::Vector2u pos);
  sf::Vector2u getPosition() const;
  void setFloorLayerId(const std::string& id);
  const std::string& getFloorLayerId() const;
  void setObjectLayerId(const std::string& id);
  const std::string& getObjectLayerId() const;
  void setBarrier(bool value);
  bool isBarrier() const;
  void setPlayer(bool value);
  bool isPlayer() const;
  void setEnemy(bool value);
  bool isEnemy() const;
  void setObject(bool value);
  bool isObject() const;
  void setNPC(bool value);
  bool isNpc() const;
};
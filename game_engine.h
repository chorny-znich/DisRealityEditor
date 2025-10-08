#pragma once
#include <disreality_engine.h>

class GameEngine : public dr::Engine
{
private:
  void createStartScreen() override;
  void init() override;
};
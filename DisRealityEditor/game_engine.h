#pragma once

#include <DisRealityGF.h>

class GameEngine : public dr::Engine {
private:
  void createStartScreen() override;
  void init() override;
};
#pragma once

#include <DisRealityGF.h>

class Game : public dr::Engine {
private:
  void createStartScreen() override;
  void init() override;
};
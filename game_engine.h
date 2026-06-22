#pragma once
#include <engine.h>

class GameEngine : public dr::Engine
{
private:
	virtual void init() override;
	virtual void createStartScreen() override;
};
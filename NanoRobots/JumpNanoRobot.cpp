#include "JumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

JumpNanoRobot::JumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(550.f * 16.f, 0.f), NANO_JUMP_OSS, 4, 654)
{
	setup(this);
}

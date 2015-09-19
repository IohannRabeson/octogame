#include "JumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

JumpNanoRobot::JumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(9800, -1400.f), NANO_JUMP_OSS, 4, 654)
{
	setup(this);
}

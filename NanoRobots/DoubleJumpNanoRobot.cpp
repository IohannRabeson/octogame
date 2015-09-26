#include "DoubleJumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

DoubleJumpNanoRobot::DoubleJumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(8236.f, -2200.f), NANO_DOUBLE_JUMP_OSS, 3, 18524)
{
	setup(this);
}

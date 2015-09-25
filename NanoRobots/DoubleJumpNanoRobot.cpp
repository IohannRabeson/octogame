#include "DoubleJumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

DoubleJumpNanoRobot::DoubleJumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_DOUBLE_JUMP_OSS, 3, 18524, sf::Vector2f())
{
	setup(this);
}

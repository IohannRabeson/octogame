#include "JumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

JumpNanoRobot::JumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(9700, -1400.f), NANO_JUMP_OSS, 4, 654, sf::Vector2f())
{
	setup(this);
}

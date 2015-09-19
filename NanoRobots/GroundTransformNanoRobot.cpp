#include "GroundTransformNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

GroundTransformNanoRobot::GroundTransformNanoRobot(void) :
	NanoRobot(sf::Vector2f(30 * 16.f, 800.f), NANO_GROUND_TRANSFORM_OSS, 4, 9854)
{
	setup(this);
}

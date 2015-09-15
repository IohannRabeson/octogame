#include "GroundTransformNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

GroundTransformNanoRobot::GroundTransformNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_GROUND_TRANSFORM_OSS, 4)
{
	setup(this);
}

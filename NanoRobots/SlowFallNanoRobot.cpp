#include "SlowFallNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

SlowFallNanoRobot::SlowFallNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_SLOW_FALL_OSS, 2, 5544, sf::Vector2f())
{
	setup(this);
}

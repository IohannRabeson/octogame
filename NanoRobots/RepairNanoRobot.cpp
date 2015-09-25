#include "RepairNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

RepairNanoRobot::RepairNanoRobot(void) :
	NanoRobot(sf::Vector2f(600.f * 16.f, 800.f), NANO_REPAIR_OSS, 8, 12542, sf::Vector2f(-2.f, 16.f))
{
	setup(this);
}

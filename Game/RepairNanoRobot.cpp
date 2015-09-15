#include "RepairNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

RepairNanoRobot::RepairNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_REPAIR_OSS, 8)
{
	setup(this);
}

#include "RepairNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

RepairNanoRobot::RepairNanoRobot(void) :
	NanoRobot(sf::Vector2f(600.f * 16.f, 800.f), NANO_REPAIR_OSS, 8, 12542, sf::Vector2f(-2.f, 16.f))
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(301.f, 133.f));
	targets.push_back(sf::Vector2f(320.f, 133.f));
	targets.push_back(sf::Vector2f(320.f, 151.f));
	targets.push_back(sf::Vector2f(307.f, 158.f));
	targets.push_back(sf::Vector2f(307.f, 170.f));
	targets.push_back(sf::Vector2f(283.f, 170.f));
	targets.push_back(sf::Vector2f(295.f, 157.f));
	targets.push_back(sf::Vector2f(295.f, 145.f));
	targets.push_back(sf::Vector2f(301.f, 145.f));
	targets.push_back(sf::Vector2f(295.f, 139.f));
	setTargets(targets, 0.6f);
}

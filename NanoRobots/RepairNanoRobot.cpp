#include "RepairNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

RepairNanoRobot::RepairNanoRobot(void) :
	NanoRobot(sf::Vector2f(600.f * 16.f, 800.f), NANO_REPAIR_OSS, 8, 12542, sf::Vector2f(-2.f, 16.f))
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(189.f, 170.f));
	targets.push_back(sf::Vector2f(214.f, 170.f));
	targets.push_back(sf::Vector2f(238.f, 189.f));
	targets.push_back(sf::Vector2f(239.f, 189.f));
	targets.push_back(sf::Vector2f(251.f, 201.f));
	targets.push_back(sf::Vector2f(251.f, 220.f));
	targets.push_back(sf::Vector2f(239.f, 220.f));
	targets.push_back(sf::Vector2f(232.f, 226.f));
	targets.push_back(sf::Vector2f(219.f, 223.f));
	targets.push_back(sf::Vector2f(214.f, 220.f));
	targets.push_back(sf::Vector2f(208.f, 226.f));
	targets.push_back(sf::Vector2f(195.f, 214.f));
	targets.push_back(sf::Vector2f(195.f, 208.f));
	targets.push_back(sf::Vector2f(189.f, 208.f));
	setTargets(targets, 0.6f);
}

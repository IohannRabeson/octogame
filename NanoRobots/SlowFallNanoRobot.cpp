#include "SlowFallNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

SlowFallNanoRobot::SlowFallNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_SLOW_FALL_OSS, 2, 5544, sf::Vector2f(0.f, -19.f))
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(98.f, 251.f));
	targets.push_back(sf::Vector2f(104.f, 254.f));
	targets.push_back(sf::Vector2f(107.f, 262.f));
	targets.push_back(sf::Vector2f(100.f, 273.f));
	targets.push_back(sf::Vector2f(91.f, 275.f));
	targets.push_back(sf::Vector2f(86.f, 274.f));
	setTargets(targets, 0.6f);
}

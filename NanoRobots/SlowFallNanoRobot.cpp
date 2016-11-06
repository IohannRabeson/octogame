#include "SlowFallNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

SlowFallNanoRobot::SlowFallNanoRobot(sf::Vector2f const & position) :
	NanoRobot(position, NANO_SLOW_FALL_OSS, 2, 5544, sf::Vector2f(0.f, -19.f), InputListener::OctoKeys::Action, 4.f)
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
	setLaserColor(sf::Color(247, 238, 80));

	setSwarmTarget(position);
}

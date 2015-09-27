#include "DoubleJumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

DoubleJumpNanoRobot::DoubleJumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(8572.f, -2700.f), NANO_DOUBLE_JUMP_OSS, 3, 18524, sf::Vector2f(0.f, -22.f))
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(288.f, 78.f));
	targets.push_back(sf::Vector2f(269.f, 64.f));
	targets.push_back(sf::Vector2f(262.f, 73.f));
	targets.push_back(sf::Vector2f(290.f, 95.f));
	targets.push_back(sf::Vector2f(293.f, 113.f));
	targets.push_back(sf::Vector2f(254.f, 83.f));
	targets.push_back(sf::Vector2f(246.f, 93.f));
	targets.push_back(sf::Vector2f(289.f, 125.f));
	targets.push_back(sf::Vector2f(279.f, 134.f));
	targets.push_back(sf::Vector2f(239.f, 104.f));
	targets.push_back(sf::Vector2f(242.f, 121.f));
	targets.push_back(sf::Vector2f(278.f, 147.f));
	targets.push_back(sf::Vector2f(276.f, 162.f));
	targets.push_back(sf::Vector2f(244.f, 138.f));
	targets.push_back(sf::Vector2f(246.f, 158.f));
	targets.push_back(sf::Vector2f(276.f, 178.f));
	targets.push_back(sf::Vector2f(276.f, 192.f));
	targets.push_back(sf::Vector2f(250.f, 173.f));
	targets.push_back(sf::Vector2f(253.f, 191.f));
	targets.push_back(sf::Vector2f(275.f, 208.f));
	targets.push_back(sf::Vector2f(273.f, 222.f));
	targets.push_back(sf::Vector2f(255.f, 208.f));
	targets.push_back(sf::Vector2f(257.f, 226.f));
	targets.push_back(sf::Vector2f(273.f, 237.f));
	targets.push_back(sf::Vector2f(271.f, 253.f));
	targets.push_back(sf::Vector2f(260.f, 244.f));
	targets.push_back(sf::Vector2f(239.f, 104.f));
	targets.push_back(sf::Vector2f(269.f, 64.f));
	setTargets(targets, 0.4f);
}

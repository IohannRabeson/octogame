#include "DoubleJumpNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

DoubleJumpNanoRobot::DoubleJumpNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_DOUBLE_JUMP_OSS, 3, 18524, sf::Vector2f())
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(450.f, 385.f));
	targets.push_back(sf::Vector2f(450.f, 394.f));
	targets.push_back(sf::Vector2f(425.f, 420.f));
	targets.push_back(sf::Vector2f(407.f, 420.f));
	targets.push_back(sf::Vector2f(394.f, 407.f));
	targets.push_back(sf::Vector2f(394.f, 389.f));
	targets.push_back(sf::Vector2f(419.f, 363.f));
	targets.push_back(sf::Vector2f(429.f, 363.f));
	setTargets(targets, 0.6f);
}

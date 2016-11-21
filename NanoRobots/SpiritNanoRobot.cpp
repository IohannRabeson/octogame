#include "SpiritNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

std::size_t SpiritNanoRobot::s_seed = 0u;

SpiritNanoRobot::SpiritNanoRobot(sf::Vector2f const & position) :
	NanoRobot(position, FOREST_SPIRIT_1_OSS, 12, s_seed++, sf::Vector2f(0.f, -28.f), InputListener::OctoKeys::Jump, 1.f)
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(284.f, 339.f));
	targets.push_back(sf::Vector2f(328.f, 369.f));
	targets.push_back(sf::Vector2f(331.f, 381.f));
	targets.push_back(sf::Vector2f(320.f, 396.f));
	targets.push_back(sf::Vector2f(308.f, 398.f));
	targets.push_back(sf::Vector2f(264.f, 367.f));
	targets.push_back(sf::Vector2f(263.f, 362.f));
	targets.push_back(sf::Vector2f(278.f, 340.f));
	setTargets(targets, 0.6f);
	setLaserColor(sf::Color::Transparent);

	setSwarmTarget(position);
	getSprite().setColor(sf::Color(255, 255, 255, 130));
}

void SpiritNanoRobot::update(sf::Time frametime)
{
	NanoRobot::update(frametime);

	float const				maxDist = 300.f;
	sf::Vector2f const &	octoPos = Progress::getInstance().getOctoPos();
	float					dist = std::sqrt(std::pow(getPosition().x - octoPos.x, 2u) + std::pow(getPosition().y - octoPos.y, 2u));

	if (dist < maxDist)
	{
		float alpha = octo::linearInterpolation(0.f, 130.f, std::max(dist / maxDist - 0.5f, 0.f));
		getSprite().setColor(sf::Color(255, 255, 255, alpha));
	}
}

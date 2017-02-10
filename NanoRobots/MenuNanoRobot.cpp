#include "MenuNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>

std::size_t MenuNanoRobot::s_seed = 0u;

MenuNanoRobot::MenuNanoRobot(sf::Vector2f const & position) :
	NanoRobot(position, FOREST_SPIRIT_2_OSS, 12, s_seed++, sf::Vector2f(0.f, -28.f), InputListener::OctoKeys::Jump, 1.f),
	m_generator("random")
{
	octo::ResourceManager &	resources = octo::Application::getResourceManager();

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

	if (m_generator.randomBool(0.5f))
		getSprite().setSpriteSheet(resources.getSpriteSheet(FOREST_SPIRIT_2_OSS));

	m_scale = m_generator.randomFloat(0.4f, 0.7f);
	m_alphaMax = m_generator.randomFloat(80.f, 150.f);
	getSprite().setScale(m_scale, m_scale);
	getSprite().setColor(sf::Color(255, 255, 255, m_alphaMax));
}

void MenuNanoRobot::update(sf::Time frametime)
{
	NanoRobot::update(frametime);

	float const				maxDist = 500.f;
	sf::Vector2f const &	octoPos = Progress::getInstance().getOctoPos();
	float					dist = std::sqrt(std::pow(getPosition().x - octoPos.x, 2u) + std::pow(getPosition().y - octoPos.y, 2u));
	float					transferProgress = getNanoEffect().getTransferProgress();
	float					alpha = m_alphaMax;
	float					scale = m_scale;

	if (transferProgress)
	{
		if (transferProgress < 0.5f)
		{
			alpha = octo::linearInterpolation(m_alphaMax, 255.f, std::min(1.f, transferProgress * 2.f));
			scale = octo::linearInterpolation(m_scale, m_scale * 2.f, std::min(1.f, transferProgress * 2.f));
		}
		else
		{
			alpha = octo::linearInterpolation(255.f, m_alphaMax, std::min(1.f, (transferProgress - 0.5f) * 2.f));
			scale = octo::linearInterpolation(m_scale * 2.f, m_scale, std::min(1.f, (transferProgress - 0.5f) * 2.f));
		}
	}
	else
	{
		if (dist < maxDist && getState() == State::FollowOcto)
			alpha = octo::linearInterpolation(0.f, m_alphaMax, dist / maxDist);
		if (getNanoEffect().getState() == NanoEffect::State::Random)
			getNanoEffect().setState(NanoEffect::State::Wait);
	}

	getSprite().setColor(sf::Color(255, 255, 255, alpha));
	getSprite().setScale(scale, scale);
}

void MenuNanoRobot::transfertToOcto(bool)
{
}

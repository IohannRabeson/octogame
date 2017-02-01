#include "BalleNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include <Interpolations.hpp>
#include <ResourceManager.hpp>
#include <Application.hpp>

BalleNanoRobot::BalleNanoRobot(sf::Vector2f const & position) :
	NanoRobot(position, NANO_JUMP_OSS, 4, 654, sf::Vector2f(0.f, -24.f), InputListener::OctoKeys::Jump, 1.f),
	m_throwPotionTimerMax(sf::seconds(0.6f))
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
	setLaserColor(sf::Color::Red);

	setSwarmTarget(position);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	m_potion.setSpriteSheet(resources.getSpriteSheet(POTION_OSS));
}

void BalleNanoRobot::update(sf::Time frametime)
{
	NanoRobot::update(frametime);
	updatePotion(frametime);
}

void BalleNanoRobot::updatePotion(sf::Time frametime)
{
	if (m_throwPotionTimer <= m_throwPotionTimerMax)
		m_throwPotionTimer += frametime;
	else if (m_throwPotionTimer > sf::Time::Zero)
		m_throwPotionTimer -= frametime;
	
	if (m_throwPotionTimer > m_throwPotionTimerMax)
		m_throwPotionTimer = m_throwPotionTimerMax;
	else if (m_throwPotionTimer < sf::Time::Zero)
		m_throwPotionTimer = sf::Time::Zero;

	float coef = m_throwPotionTimer / m_throwPotionTimerMax;

	if (coef <= 1.f / 6.f)
		m_potion.setScale(coef * 6.f, coef * 6.f);
	else if (coef > 9.f / 10.f)
		m_potion.setScale((1.f - coef) * 10.f, (1.f - coef) * 10.f);
	m_potion.setRotation(720.f * coef);

	m_potion.setPosition(octo::cosinusInterpolation(getPosition() + sf::Vector2f(-4.f, 50.f), m_octoPosition, coef));
}

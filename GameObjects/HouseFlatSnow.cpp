#include "HouseFlatSnow.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

HouseFlatSnow::HouseFlatSnow(void) :
	SimpleObject(HOUSE_FLAT_SNOW_OSS, PERSISTENCE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);
	//setupSmoke();

	setupBox(this, static_cast<std::size_t>(GameObjectType::HouseFlatSnow), static_cast<std::size_t>(GameObjectType::Player));
}

HouseFlatSnow::~HouseFlatSnow(void)
{}

void HouseFlatSnow::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(18, 14, 66, 150));

	m_smoke2.setup(sf::Vector2f(5.f, 5.f));
	m_smoke2.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke2.setEmitTimeRange(0.2f, 0.3f);
	m_smoke2.setGrowTimeRange(0.4f, 0.6f);
	m_smoke2.setLifeTimeRange(0.6f, 0.8f);
	m_smoke2.setScaleFactor(10.f);
	m_smoke2.setDispersion(80.f);
	m_smoke2.setColor(sf::Color(18, 14, 66, 150));
}

void HouseFlatSnow::startBalle(void)
{
	SimpleObject::startBalle();
	m_smoke.setVelocity(sf::Vector2f(0.f, -200.f));
	m_smoke.setEmitTimeRange(0.05f, 0.1f);
	m_smoke.setScaleFactor(15.f);
	m_smoke.setDispersion(150.f);
	m_smoke2.setVelocity(sf::Vector2f(0.f, -200.f));
	m_smoke2.setEmitTimeRange(0.05f, 0.1f);
	m_smoke2.setScaleFactor(15.f);
	m_smoke2.setDispersion(150.f);
}

void HouseFlatSnow::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 1820.f));
	m_smoke.setPosition(position + sf::Vector2f(315.f, -350.f));
	m_smoke2.setPosition(position + sf::Vector2f(235.f, -325.f));
}

void HouseFlatSnow::update(sf::Time frameTime)
{
	/*
	m_smoke.update(frameTime);
	m_smoke2.update(frameTime);
	sf::Shader & shader = getShader();

	if (getStartBalle())
	{
		float intensity;
		sf::Time step = getEffectDuration() / 6.f;
		if (getTimer() < step)
			intensity = octo::linearInterpolation(1.f, 0.1f, getTimer() / step);
		else if (getTimer() < 3.f * step)
			intensity = octo::linearInterpolation(0.1f, 0.02f, (getTimer() - step) / (2.f * step));
		else if (getTimer() < 5.f * step)
			intensity = octo::linearInterpolation(0.02f, 0.1f, (getTimer() - 3.f * step) / (2.f * step));
		else
			intensity = octo::linearInterpolation(0.1f, 1.f, (getTimer() - 5.f * step) / step);
		shader.setParameter("intensity", intensity);
	}
	if (getCollideWithOcto())
	{
		m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke.setEmitTimeRange(0.2f, 0.3f);
		m_smoke.setScaleFactor(10.f);
		m_smoke.setDispersion(80.f);
		m_smoke2.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke2.setEmitTimeRange(0.2f, 0.3f);
		m_smoke2.setScaleFactor(10.f);
		m_smoke2.setDispersion(80.f);
	}
	*/
	SimpleObject::update(frameTime);
}

void HouseFlatSnow::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
}

void HouseFlatSnow::drawFront(sf::RenderTarget& render, sf::RenderStates) const
{
	m_smoke.draw(render);
	m_smoke2.draw(render);
}

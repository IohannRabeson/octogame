#include "Bouibouik.hpp"
#include <Interpolations.hpp>

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_OSS, PERSISTENCE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 1u)}, octo::LoopMode::Loop);
	setupSmoke();

	setupBox(this, static_cast<std::uint32_t>(GameObjectType::Bouibouik), static_cast<std::uint32_t>(GameObjectType::Player));
}

Bouibouik::~Bouibouik(void)
{
}

void Bouibouik::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(18, 14, 66, 150));
}

void Bouibouik::startBalle(void)
{
	SimpleObject::startBalle();
	m_smoke.setVelocity(sf::Vector2f(0.f, -200.f));
	m_smoke.setEmitTimeRange(0.05f, 0.1f);
	m_smoke.setScaleFactor(15.f);
	m_smoke.setDispersion(150.f);
}

void Bouibouik::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 370.f));
	m_smoke.setPosition(position - sf::Vector2f(-55.f, 350.f));
}

void Bouibouik::update(sf::Time frameTime)
{
	m_smoke.update(frameTime);
	sf::Shader & shader = getShader();

	if (getStartBalle())
	{
		float intensity;
		sf::Time step = getEffectDuration() / 6.f;
		if (getTimer() < step)
			intensity = octo::linearInterpolation(1.f, 0.1f, getTimer() / step);
		else if (getTimer() < 3.f * step)
			intensity = octo::linearInterpolation(0.1f, 0.02f, (getTimer() - step) / (3.f * step));
		else if (getTimer() < 5.f * step)
			intensity = octo::linearInterpolation(0.02f, 0.1f, (getTimer() - 3.f * step) / (5.f * step));
		else
			intensity = octo::linearInterpolation(0.1f, 1.f, (getTimer() - 5.f * step) / (getEffectDuration()));
		shader.setParameter("intensity", intensity);
	}
	if (getCollideWithOcto())
	{
		m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke.setEmitTimeRange(0.2f, 0.3f);
		m_smoke.setScaleFactor(10.f);
		m_smoke.setDispersion(80.f);
	}
	SimpleObject::update(frameTime);
}

void Bouibouik::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
	m_smoke.draw(render);
}

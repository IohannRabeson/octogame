#include "Bouibouik.hpp"

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 1u)}, octo::LoopMode::Loop);
	setupSmoke();
}

Bouibouik::~Bouibouik(void)
{}

void Bouibouik::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(155, 155, 155, 150));
}

void Bouibouik::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 370.f));
	m_smoke.setPosition(position - sf::Vector2f(-55.f, 350.f));
}

void Bouibouik::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);
	m_smoke.update(frameTime);
}

void Bouibouik::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
	m_smoke.draw(render);
}

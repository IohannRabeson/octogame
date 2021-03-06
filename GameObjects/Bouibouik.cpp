#include "Bouibouik.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_BACK_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);
	setupSmoke();

	octo::ResourceManager & resources = octo::Application::getResourceManager();

	typedef octo::SpriteAnimation::Frame	Frame;
	m_animationFront.setFrames({Frame(sf::seconds(0.2f), 0u),
								Frame(sf::seconds(0.2f), 1u),
								Frame(sf::seconds(0.2f), 2u),
								Frame(sf::seconds(0.2f), 1u)});
	m_animationFront.setLoop(octo::LoopMode::Loop);
	m_spriteFront.setSpriteSheet(resources.getSpriteSheet(BOUIBOUIK_FRONT_OSS));
	m_spriteFront.setScale(1.f, 1.f);
	m_spriteFront.setAnimation(m_animationFront);
	m_spriteFront.play();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Bouibouik), static_cast<std::size_t>(GameObjectType::Player));
}

void Bouibouik::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(sf::Vector2f(80.f, 0.f));
	m_smoke.setColor(sf::Color(18, 14, 66, 150));

	m_smoke2.setup(sf::Vector2f(5.f, 5.f));
	m_smoke2.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke2.setEmitTimeRange(0.2f, 0.3f);
	m_smoke2.setGrowTimeRange(0.4f, 0.6f);
	m_smoke2.setLifeTimeRange(0.6f, 0.8f);
	m_smoke2.setScaleFactor(10.f);
	m_smoke2.setDispersion(sf::Vector2f(80.f, 0.f));
	m_smoke2.setColor(sf::Color(18, 14, 66, 150));
}

void Bouibouik::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 370.f));
	m_spriteFront.setPosition(position - sf::Vector2f(0.f, 370.f));
	m_smoke.setPosition(position + sf::Vector2f(315.f, -350.f));
	m_smoke2.setPosition(position + sf::Vector2f(235.f, -325.f));
}

void Bouibouik::update(sf::Time frameTime)
{
	m_smoke.update(frameTime);
	m_smoke2.update(frameTime);
	if (getCollideWithOcto())
	{
		m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke.setEmitTimeRange(0.2f, 0.3f);
		m_smoke.setScaleFactor(10.f);
		m_smoke.setDispersion(sf::Vector2f(80.f, 0.f));
		m_smoke2.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke2.setEmitTimeRange(0.2f, 0.3f);
		m_smoke2.setScaleFactor(10.f);
		m_smoke2.setDispersion(sf::Vector2f(80.f, 0.f));
	}
	SimpleObject::update(frameTime);
	m_spriteFront.update(frameTime);
}

void Bouibouik::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
}

void Bouibouik::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_spriteFront, states);
	m_smoke.draw(render);
	m_smoke2.draw(render);
}

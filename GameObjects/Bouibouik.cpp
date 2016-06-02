#include "Bouibouik.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_BACK_OSS, PERSISTENCE_FRAG, 20.f),
	m_sound(true)
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

void Bouibouik::startBalle(void)
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
	//sf::Shader & shader = getShader();

	if (getStartBalle())
	{
		playSound();
		//float intensity;
		//sf::Time step = getEffectDuration() / 6.f;
		//if (getTimer() < step)
		//	intensity = octo::linearInterpolation(1.f, 0.1f, getTimer() / step);
		//else if (getTimer() < 3.f * step)
		//	intensity = octo::linearInterpolation(0.1f, 0.02f, (getTimer() - step) / (2.f * step));
		//else if (getTimer() < 5.f * step)
		//	intensity = octo::linearInterpolation(0.02f, 0.1f, (getTimer() - 3.f * step) / (2.f * step));
		//else
		//	intensity = octo::linearInterpolation(0.1f, 1.f, (getTimer() - 5.f * step) / step);
		//shader.setParameter("intensity", intensity);
	}
	else
		m_sound = true;
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
	SimpleObject::update(frameTime);
	m_spriteFront.update(frameTime);
}

void Bouibouik::playSound(void)
{
	if (m_sound)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();
		audio.playSound(resources.getSound(OCTO_COUGH_OGG), 0.7f);
		m_sound = false;
	}
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

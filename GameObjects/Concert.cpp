#include "Concert.hpp"
#include <Interpolations.hpp>
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Camera.hpp>

Concert::Concert(void) :
	SimpleObject(CONCERT_OSS, CONCERT_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 3u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::Concert), static_cast<std::size_t>(GameObjectType::Player));
	getSprite().setScale(sf::Vector2f(0.8f, 0.8f));
	m_particles.canEmit(true);
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	getShader().setParameter("resolution", rect.width, rect.height);
}

Concert::~Concert(void)
{}

void Concert::update(sf::Time frameTime)
{
	sf::Shader & shader = getShader();

	if (getStartBalle())
	{
		float time;
		if (getTimer() < getEffectDuration() / 2.f)
			time = octo::linearInterpolation(0.f, 1.f, getTimer() / (getEffectDuration() / 2.f));
		else
			time = octo::linearInterpolation(1.f, 0.f, (getTimer() - getEffectDuration() / 2.f) / (getEffectDuration() / 2.f));
		shader.setParameter("time", time);
	}
	SimpleObject::update(frameTime);
	m_particles.update(frameTime);
}

void Concert::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, getSprite().getGlobalSize().x));
	m_particles.setEmitter(position + sf::Vector2f(100.f, -170.f));
}

void Concert::draw(sf::RenderTarget &, sf::RenderStates) const
{}

void Concert::drawFront(sf::RenderTarget & render, sf::RenderStates) const
{
	render.draw(getSprite());
	m_particles.draw(render);
}

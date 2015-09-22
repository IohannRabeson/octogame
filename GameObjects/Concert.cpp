#include "Concert.hpp"
#include <Interpolations.hpp>
#include <SFML/Graphics.hpp>

Concert::Concert(void) :
	SimpleObject(TENT_OSS, PIXELATE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::Concert), static_cast<std::size_t>(GameObjectType::Player));
	adjustBox(sf::Vector2f(30.f, 30.f), 170.f);
	m_particles.canEmit(true);
}

Concert::~Concert(void)
{}

void Concert::update(sf::Time frameTime)
{
	sf::Shader & shader = getShader();

	if (getStartBalle())
	{
		float intensity;
		if (getTimer() < getEffectDuration() / 2.f)
			intensity = octo::linearInterpolation(0.f, 0.02f, getTimer() / (getEffectDuration() / 2.f));
		else
			intensity = octo::linearInterpolation(0.02f, 0.f, (getTimer() - getEffectDuration() / 2.f) / (getEffectDuration() / 2.f));
		shader.setParameter("pixel_threshold", intensity);
	}
	SimpleObject::update(frameTime);
	m_particles.update(frameTime);
}

void Concert::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 180.f));
	m_particles.setEmitter(position);
}

void Concert::draw(sf::RenderTarget &, sf::RenderStates) const
{}

void Concert::drawFront(sf::RenderTarget & render, sf::RenderStates) const
{
	render.draw(getSprite());
	m_particles.draw(render);
}

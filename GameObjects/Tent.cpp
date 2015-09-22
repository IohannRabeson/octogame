#include "Tent.hpp"
#include <Interpolations.hpp>

Tent::Tent(void) :
	SimpleObject(TENT_OSS, PIXELATE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::Tent), static_cast<std::size_t>(GameObjectType::Player));
	adjustBox(sf::Vector2f(30.f, 30.f), 170.f);
}

Tent::~Tent(void)
{}

void Tent::update(sf::Time frameTime)
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
}

void Tent::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 180.f));
}

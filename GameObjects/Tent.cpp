#include "Tent.hpp"
#include <Interpolations.hpp>
#include <SFML/Graphics.hpp>

Tent::Tent(void) :
	SimpleObject(TENT_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::Tent), static_cast<std::size_t>(GameObjectType::Player));
	adjustBox(sf::Vector2f(30.f, 30.f), 170.f);
}

void Tent::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 180.f));
}

void Tent::draw(sf::RenderTarget &, sf::RenderStates) const
{}

void Tent::drawFront(sf::RenderTarget & render, sf::RenderStates) const
{
	render.draw(getSprite());
}

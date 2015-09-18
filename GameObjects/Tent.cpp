#include "Tent.hpp"

Tent::Tent(void) :
	SimpleObject(TENT_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);
}

Tent::~Tent(void)
{}

void Tent::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 180.f));
}

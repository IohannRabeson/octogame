#include "Bouibouik.hpp"

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 1u)}, octo::LoopMode::Loop);
}

Bouibouik::~Bouibouik(void)
{}

void Bouibouik::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 400.f));
}

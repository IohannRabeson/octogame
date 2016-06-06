#include "HouseFlatSnow.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

HouseFlatSnow::HouseFlatSnow(void) :
	SimpleObject(HOUSE_FLAT_SNOW_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u), Frame(sf::seconds(0.2f), 1u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::HouseFlatSnow), static_cast<std::size_t>(GameObjectType::Player));
}

void HouseFlatSnow::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 1820.f));
}

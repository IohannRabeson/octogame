#include "WeirdHouseSnow.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

WeirdHouseSnow::WeirdHouseSnow(void) :
	SimpleObject(WEIRD_HOUSE_SNOW_OSS)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::WeirdHouseSnow), static_cast<std::size_t>(GameObjectType::Player));
}

void WeirdHouseSnow::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 1140.f));
}

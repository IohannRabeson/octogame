#include "WeirdHouseSnow.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

WeirdHouseSnow::WeirdHouseSnow(void) :
	SimpleObject(WEIRD_HOUSE_SNOW_OSS, PERSISTENCE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::WeirdHouseSnow), static_cast<std::size_t>(GameObjectType::Player));
}

WeirdHouseSnow::~WeirdHouseSnow(void)
{}

void WeirdHouseSnow::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 1140.f));
}

void WeirdHouseSnow::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);
}

void WeirdHouseSnow::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
}

#include "EngineSnow.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

EngineSnow::EngineSnow(void) :
	SimpleObject(ENGINE_SNOW_OSS, PERSISTENCE_FRAG, 20.f)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 3u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 0u)},
					octo::LoopMode::Loop);


	setupBox(this, static_cast<std::size_t>(GameObjectType::EngineSnow), static_cast<std::size_t>(GameObjectType::Player));
}

EngineSnow::~EngineSnow(void)
{}

void EngineSnow::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 740.f));
}

void EngineSnow::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);
}

void EngineSnow::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
}

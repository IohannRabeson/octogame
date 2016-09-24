#include "WaterHouseBroken.hpp"

WaterHouseBroken::WaterHouseBroken(void) :
	AIdleNpc(WATER_HOUSE_BROKEN_OSS, false, false)
{
    setType(GameObjectType::WaterHouseBroken);
	setSize(sf::Vector2f(512.f, 670.f));
	setOrigin(sf::Vector2f(51.f, 169.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setup();
}

void WaterHouseBroken::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

#include "WellKeeperNpc.hpp"

WellKeeperNpc::WellKeeperNpc(void) :
	AIdleNpc(NPC_WELL_KEEPER_OSS)
{
	setSize(sf::Vector2f(95.f, 170.f));
	setOrigin(sf::Vector2f(50.f, 83.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(20.f, 0.f));
	setup();
}

void WellKeeperNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

#include "WellKeeperNpc.hpp"

WellKeeperNpc::WellKeeperNpc(void) :
	AIdleNpc(NPC_WELL_KEEPER_OSS)
{
	setType(GameObjectType::WellKeeperNpc);
	setSize(sf::Vector2f(35.f, 156.f));
	setOrigin(sf::Vector2f(87.f, 100.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
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

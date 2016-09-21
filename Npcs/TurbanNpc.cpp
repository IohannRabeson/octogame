#include "TurbanNpc.hpp"

TurbanNpc::TurbanNpc(void) :
	AIdleNpc(NPC_TURBAN_OSS)
{
	setType(GameObjectType::TurbanNpc);
	setSize(sf::Vector2f(35.f, 162.f));
	setOrigin(sf::Vector2f(87.f, 94.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void TurbanNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

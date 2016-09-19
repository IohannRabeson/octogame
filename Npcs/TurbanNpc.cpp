#include "TurbanNpc.hpp"

TurbanNpc::TurbanNpc(void) :
	AIdleNpc(NPC_TURBAN_OSS)
{
	setSize(sf::Vector2f(75.f, 170.f));
	setOrigin(sf::Vector2f(50.f, 84.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
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

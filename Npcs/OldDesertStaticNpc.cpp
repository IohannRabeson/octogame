#include "OldDesertStaticNpc.hpp"

OldDesertStaticNpc::OldDesertStaticNpc(void) :
	AIdleNpc(NPC_OLD_MAN_DESERT_STATIC_OSS)
{
	setSize(sf::Vector2f(32.f, 168.f));
	setOrigin(sf::Vector2f(85.f, 25.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void OldDesertStaticNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

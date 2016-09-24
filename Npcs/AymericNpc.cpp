#include "AymericNpc.hpp"

AymericNpc::AymericNpc(void) :
	ASpecialNpc(AYMERIC_OSS, false)
{
	setType(GameObjectType::AymericNpc);
	setSize(sf::Vector2f(25.f, 180.f));
	setOrigin(sf::Vector2f(129.f, 82.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void AymericNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

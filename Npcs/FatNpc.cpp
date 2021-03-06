#include "FatNpc.hpp"

FatNpc::FatNpc(void) :
	AWalkNpc(NPC_FAT_OSS)
{
	setType(GameObjectType::FatNpc);
	setSize(sf::Vector2f(32.f, 218.f));
	setOrigin(sf::Vector2f(86.f, 50.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void FatNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.6f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {4u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

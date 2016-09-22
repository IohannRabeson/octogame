#include "FabienNpc.hpp"

FabienNpc::FabienNpc(void) :
	AFlyNpc(FABIEN_OSS, true)
{
	setType(GameObjectType::FabienNpc);
	setSize(sf::Vector2f(257.f, 20.f));
	setOrigin(sf::Vector2f(51.f, 205.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setup();
}

void FabienNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::NoLoop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	getSpecial2Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial2Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
	setNextEvent(Special1);
}

#include "TheoNpc.hpp"

TheoNpc::TheoNpc(void) :
	ASpecialNpc(THEO_OSS)
{
    setType(GameObjectType::TheoNpc);
	setSize(sf::Vector2f(70.f, 185.f));
	setOrigin(sf::Vector2f(68.f, 59.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setup();
}

void TheoNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

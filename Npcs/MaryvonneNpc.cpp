#include "MaryvonneNpc.hpp"

MaryvonneNpc::MaryvonneNpc(void) :
	AIdleNpc(MARYVONNE_OSS)
{
    setType(GameObjectType::MaryvonneNpc);
	setSize(sf::Vector2f(32.f, 204.f));
	setOrigin(sf::Vector2f(90.f, 85.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-0.f, -50.f));
	setup();
	setIsZoomable(false);
}

void MaryvonneNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

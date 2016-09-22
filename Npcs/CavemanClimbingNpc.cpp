#include "CavemanClimbingNpc.hpp"

CavemanClimbingNpc::CavemanClimbingNpc(void) :
	AIdleNpc(CAVEMAN_CLIMBING_OSS)
{
	setType(GameObjectType::CavemanClimbingNpc);
	setSize(sf::Vector2f(75.f, 265.f));
	setOrigin(sf::Vector2f(60.f, 1050.f));
	setScale(0.8f);
	setup();
}

void CavemanClimbingNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

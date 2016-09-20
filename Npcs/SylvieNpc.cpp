#include "SylvieNpc.hpp"

SylvieNpc::SylvieNpc(void) :
	AIdleNpc(SYLVIE_OSS)
{
	setSize(sf::Vector2f(107.f, 205.f));
	setOrigin(sf::Vector2f(37.f, 82.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void SylvieNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

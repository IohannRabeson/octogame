#include "VinceNpc.hpp"

VinceNpc::VinceNpc(void) :
	AIdleNpc(VINCE_OSS, false)
{
	setSize(sf::Vector2f(26.f, 169.f));
	setOrigin(sf::Vector2f(98.f, 739.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -40.f));
	setup();
}

void VinceNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

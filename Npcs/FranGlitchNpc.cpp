#include "FranGlitchNpc.hpp"

FranGlitchNpc::FranGlitchNpc(void) :
	ADisappearNpc(FRAN_GLITCH_OSS, 30.f, 120.f, false)
{
	setSize(sf::Vector2f(20.f, 194.f));
	setOrigin(sf::Vector2f(140.f, 56.f));
	setScale(0.8f);
	setup();
}

void FranGlitchNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

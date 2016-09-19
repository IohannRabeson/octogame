#include "JuGlitchNpc.hpp"

JuGlitchNpc::JuGlitchNpc(void) :
	ADisappearNpc(JU_GLITCH_OSS, 30.f, 120.f, false)
{
	setSize(sf::Vector2f(50.f, 130.f));
	setOrigin(sf::Vector2f(120.f, 87.f));
	setup();
}

void JuGlitchNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

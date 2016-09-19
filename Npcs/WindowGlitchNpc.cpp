#include "WindowGlitchNpc.hpp"

WindowGlitchNpc::WindowGlitchNpc(void) :
	ADisappearNpc(WINDOW_GLITCH_OSS, 10.f, 40.f)
{
	setSize(sf::Vector2f(25.f, 375.f));
	setOrigin(sf::Vector2f(125.f, 165.f));
	setScale(0.8f);
	setup();
}

void WindowGlitchNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(3.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.0f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(5.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(7.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

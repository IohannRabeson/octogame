#include "AnthemJungle.hpp"

AnthemJungle::AnthemJungle(void) :
	ASpecialNpc(ANTHEM_JUNGLE_OSS)
{
	setSize(sf::Vector2f(33.f, 181.f));
	setOrigin(sf::Vector2f(138.f, 81.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void AnthemJungle::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

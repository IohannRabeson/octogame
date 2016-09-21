#include "SylvieNpc.hpp"

SylvieNpc::SylvieNpc(void) :
	ASwimNpc(SYLVIE_OSS, true, false)
{
	setSize(sf::Vector2f(107.f, 205.f));
	setOrigin(sf::Vector2f(37.f, 82.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setVelocity(15.f);
	setup();
}

void SylvieNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

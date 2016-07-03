#include "LuGlitchNpc.hpp"
#include "RectangleShape.hpp"

LuGlitchNpc::LuGlitchNpc(void) :
	DisappearNpc(LU_GLITCH_OSS, 30.f, 120.f, false)
{
	setSize(sf::Vector2f(25.f, 75.f));
	setOrigin(sf::Vector2f(125.f, 158.f));
	setScale(0.8f);
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void LuGlitchNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

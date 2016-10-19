#include "OctoDeathNpc.hpp"
#include "RectangleShape.hpp"

#include <iostream>

OctoDeathNpc::OctoDeathNpc(float waterLevel, sf::Color waterColor) :
	ASinkNpc(OCTO_DEATH_HELMET_OSS, waterLevel, waterColor)
{
	setType(GameObjectType::OctoDeathNpc);
	setSize(sf::Vector2f(38.f, 20.f));
	setOrigin(sf::Vector2f(31.f, 18.f));
	setScale(0.6f);
	setTimerMax(sf::seconds(8.0f));
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::OctoDeathNpc), static_cast<std::size_t>(GameObjectType::OctoDeathNpc));
	getBox()->setType(AShape::Type::e_kinematic);
}

void OctoDeathNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

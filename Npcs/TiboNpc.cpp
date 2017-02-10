#include "TiboNpc.hpp"

TiboNpc::TiboNpc(void) :
	ASpecialNpc(TIBO_OSS, false)
{
    setType(GameObjectType::TiboNpc);
	setSize(sf::Vector2f(56.f, 143.f));
	setOrigin(sf::Vector2f(106.f, 51.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
	setIsZoomable(false);
}

void TiboNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {23u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

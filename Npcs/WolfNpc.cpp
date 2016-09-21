#include "WolfNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

WolfNpc::WolfNpc(void) :
	AWalkNpc(WOLF_OSS)
{
	setType(GameObjectType::WolfNpc);
	setSize(sf::Vector2f(99.f, 110.f));
	setOrigin(sf::Vector2f(95.f, 82.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setTimerMax(sf::seconds(10.0f));
	setup();
}

void WolfNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.2f), {40u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {41u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {42u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {43u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {44u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {39u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.8f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {23u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {24u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {25u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

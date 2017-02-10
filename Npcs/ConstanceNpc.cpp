#include "ConstanceNpc.hpp"

ConstanceNpc::ConstanceNpc(void) :
	ASpecialNpc(CONSTANCE_OSS, false)
{
	setType(GameObjectType::ConstanceNpc);
	setSize(sf::Vector2f(26.f, 200.f));
	setOrigin(sf::Vector2f(87.f, 51.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void ConstanceNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	m_smoke.setup(sf::Vector2f(2.5f, 2.5f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -50.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(sf::Vector2f(80.f, 0.f));
	m_smoke.setColor(sf::Color(255, 255, 255, 150));
}

void ConstanceNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Idle)
	{
		m_smoke.setPosition(ASpecialNpc::getPosition() + sf::Vector2f(40.f, 140.f));
		m_smoke.setVelocity(sf::Vector2f(0.f, -30.f));
	}
	else
	{
		m_smoke.setPosition(ASpecialNpc::getPosition() + sf::Vector2f(-10.f, 90.f));
		m_smoke.setVelocity(sf::Vector2f(-50.f, 0.f));
	}
	m_smoke.update(frametime);

	ASpecialNpc::update(frametime);
}

void ConstanceNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ASpecialNpc::draw(render, states);
	m_smoke.draw(render);
}

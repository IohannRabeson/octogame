#include "FranfranNpc.hpp"
#include "Progress.hpp"

FranfranNpc::FranfranNpc(void) :
	ASpecialNpc(FRANFRAN_OSS, false, true),
	m_puffTimerMin(sf::seconds(0.4f)),
	m_puffTimerMax(sf::seconds(1.f))
{
	setType(GameObjectType::FranfranNpc);
	setSize(sf::Vector2f(69.f, 230.f));
	setOrigin(sf::Vector2f(56.f, 88.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();

	m_smokePosition = sf::Vector2f(25.f, 170.f) * 0.8f; // Multiply by scale
	m_puffPosition = sf::Vector2f(145.f, 160.f) * 0.8f;
}

void FranfranNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()})
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	m_smoke.setup(sf::Vector2f(2.5f, 2.5f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -30.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(255, 255, 255, 150));

	m_puff.setup(sf::Vector2f(2.5f, 2.5f));
	m_puff.setVelocity(sf::Vector2f(50.f, -60.f));
	m_puff.setEmitTimeRange(0.05f, 0.1f);
	m_puff.setGrowTimeRange(0.4f, 0.6f);
	m_puff.setLifeTimeRange(0.6f, 0.8f);
	m_puff.setScaleFactor(15.f);
	m_puff.setDispersion(120.f);
	m_puff.setColor(sf::Color(255, 255, 255, 150));

	if (Progress::getInstance().canRepairShip())
		setCurrentText(1u);
}

void FranfranNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();


	if (sprite.getCurrentEvent() == Idle)
	{
		m_smoke.setPosition(m_smokePosition - getSprite().getOrigin() + sprite.getPosition());
		m_smoke.setCanEmit(true);
		m_puffTimer = sf::Time::Zero;
	}
	else
	{
		m_puffTimer += frametime;
		m_smoke.setCanEmit(false);
		if (m_puffTimer >= m_puffTimerMin && m_puffTimer <= m_puffTimerMax)
			m_puff.setCanEmit(true);
		else
			m_puff.setCanEmit(false);
		m_puff.setPosition(m_puffPosition - sprite.getOrigin() + sprite.getPosition());
	}

	m_smoke.update(frametime);
	m_puff.update(frametime);

	ASpecialNpc::update(frametime);
}

void FranfranNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ASpecialNpc::draw(render, states);
	m_smoke.draw(render);
	m_puff.draw(render);
}

#include "TVScreen.hpp"
#include "PostEffectLayer.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Interpolations.hpp>
#include <Camera.hpp>

TVScreen::TVScreen(void) :
	ANpc(TV_OSS),
	m_shader(PostEffectLayer::getInstance().getShader(DUPLICATE_SCREEN_FRAG)),
	m_timer(sf::Time::Zero),
	m_duration(sf::seconds(6.f)),
	m_startZoom(false),
	m_reverse(true)
{
	setSize(sf::Vector2f(25.f, 140.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -10.f));
	setTimerMax(sf::seconds(8.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));

	m_tvScreen.width = 480.f;
	m_tvScreen.height = 270.f;
	m_shader.setParameter("offset", 1.f / 400.f);
	sf::Transform kernel(
			-1.f, -1.f, -1.f,
			-1.f, 8.f, -1.f,
			-1.f, -1.f, -1.f
		);
	m_shader.setParameter("kernel", kernel);
	m_shader.setParameter("reverse", 0.0);
}

void TVScreen::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void TVScreen::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	setMachine(machine);
	setNextEvent(Idle);
}

#include <iostream>
void TVScreen::update(sf::Time frametime)
{
	ANpc::update(frametime);
	m_tvScreen.left = ANpc::getPosition().x;
	m_tvScreen.top = ANpc::getPosition().y;
	sf::Vector2f pos;
	sf::Vector2f size;

	sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	if (screen.intersects(m_tvScreen))
	{
		PostEffectLayer::getInstance().enableShader(DUPLICATE_SCREEN_FRAG, true);
		float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
		float width = octo::Application::getGraphicsManager().getVideoMode().width;
		float height = octo::Application::getGraphicsManager().getVideoMode().height;
		pos = sf::Vector2f(((m_tvScreen.left - screen.left) * zoomFactor) / width, (height + (-m_tvScreen.top + screen.top) * zoomFactor) / height);
		size = sf::Vector2f(m_tvScreen.width * zoomFactor / width, m_tvScreen.height * zoomFactor / height);
	}
	else
	{
		PostEffectLayer::getInstance().enableShader(DUPLICATE_SCREEN_FRAG, false);
	}
	if (m_startZoom)
	{
		m_timer += frametime;
		if (m_timer >= m_duration)
		{
			m_startZoom = false;
			m_shader.setParameter("reverse", m_reverse);
			m_reverse = !m_reverse;
			m_timer = sf::Time::Zero;
		}
	}
	sf::Vector2f sub_pos = sf::Vector2f(pos.x + pos.x * size.x, pos.y + pos.y * size.y);
	sf::Vector2f sub_size = sf::Vector2f(size.x * size.x, size.y * size.y);
	sub_pos = octo::linearInterpolation(sub_pos, pos, std::min(1.f, m_timer / m_duration));
	sub_size = octo::linearInterpolation(sub_size, size, std::min(1.f, m_timer / m_duration));
	pos = octo::linearInterpolation(pos, sf::Vector2f(0.f, 0.f), std::min(1.f, m_timer / m_duration));
	size = octo::linearInterpolation(size, sf::Vector2f(1.f, 1.f), std::min(1.f, m_timer / m_duration));
	m_shader.setParameter("bot_left", pos);
	m_shader.setParameter("top_right", pos.x + size.x, pos.y + size.y);
	m_shader.setParameter("size", size);
	m_shader.setParameter("sub_bot_left", sub_pos);
	m_shader.setParameter("sub_top_right", sub_pos.x + sub_size.x, sub_pos.y + sub_size.y);
	m_shader.setParameter("sub_size", sub_size);
}

void TVScreen::updateState(void)
{}

void TVScreen::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_startZoom = true;
}

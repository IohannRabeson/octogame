#include "KonamiCode.hpp"
#include "PostEffectLayer.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

KonamiCode::KonamiCode(void) :
	m_index(0u),
	m_shader(PostEffectLayer::getInstance().getShader(CIRCLE_RAINBOW_FRAG)),
	m_state(Fail),
	m_timer(sf::Time::Zero),
	m_timerEnd(sf::Time::Zero),
	m_timerMax(sf::seconds(6.f))
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	m_shader.setUniform("color_size", 0.0005f);
	m_shader.setUniform("fade_out_size", 100.f);

	//Up;
	//Up;
	//Down;
	//Down;
	//Left;
	//Right;
	//Left;
	//Right;
	//B;
	//A;
}

KonamiCode::~KonamiCode(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void KonamiCode::update(sf::Time frameTime, sf::Vector2f const & position)
{
	if (m_state == Start)
		m_state = Success;
	else if (m_state == Success)
	{
		m_timer += frameTime;
		sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
		m_shader.setUniform("position", sf::Vector2f(position.x - screen.left, octo::Application::getGraphicsManager().getVideoMode().height - position.y + screen.top));
		m_shader.setUniform("time", 2.f * m_timer / m_timerMax);
		m_shader.setUniform("radius", (m_timer / m_timerMax) * 2000.f);
		if (m_timer > m_timerMax)
		{
			m_state = End;
			m_timer -= m_timerMax;
		}
	}
	else if (m_state == End)
	{
		m_timer += frameTime;
		m_timerEnd += frameTime;
		sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
		m_shader.setUniform("position", sf::Vector2f(position.x - screen.left, octo::Application::getGraphicsManager().getVideoMode().height - position.y + screen.top));
		m_shader.setUniform("time", 2.f * m_timer / m_timerMax);
		m_shader.setUniform("end_alpha", 1.f - (m_timerEnd / m_timerMax));
		if (m_timerEnd > m_timerMax)
		{
			PostEffectLayer::getInstance().enableShader(CIRCLE_RAINBOW_FRAG, false);
			m_state = Fail;
		}
	}
	if (m_index == 10u)
	{
		m_state = Start;
		PostEffectLayer::getInstance().enableShader(CIRCLE_RAINBOW_FRAG, true);
		m_index = 0u;
		m_timer = sf::Time::Zero;
		m_timerEnd = sf::Time::Zero;
		m_shader.setUniform("end_alpha", 1.f);
	}
}

void KonamiCode::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	(void)render;
	(void)states;
}

bool KonamiCode::canStartEvent()
{
	if (m_state == Start)
		return true;
	return false;
}

bool KonamiCode::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			(m_index == 4u || m_index == 6u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::Right:
			(m_index == 5u || m_index == 7u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::Up:
			(m_index == 0u || m_index == 1u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::Down:
			(m_index == 2u || m_index == 3u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::A:
			m_index == 9u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::B:
			m_index == 8u ? m_index++ : m_index = 0u;
			break;
		default:
			m_index = 0u;
			break;
	}
	return true;
}

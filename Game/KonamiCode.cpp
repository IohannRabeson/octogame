#include "KonamiCode.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

KonamiCode::KonamiCode(void) :
	m_index(0u),
	m_shaderIndex(0u),
	m_state(Fail),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(5.f))
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(KONAMI_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));

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

void KonamiCode::update(sf::Time frameTime)
{
	if (m_index == 10u)
		m_state = Success;
	if (m_state == Success)
	{
		octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
		postEffect.enableEffect(m_shaderIndex, true);
		m_position = sf::Vector2f(1500.f, 500.f); //TODO set position from octo
		m_shader.setParameter("position", m_position.x, m_position.y);
		m_timer += frameTime;
		if (m_timer > m_timerMax)
		{
			//m_state = Fail;
			m_timer = sf::Time::Zero;
			postEffect.enableEffect(m_shaderIndex, false);
		}
		m_shader.setParameter("time", m_timer / m_timerMax);
		m_shader.setParameter("radius", m_timer / m_timerMax * 2000.f);
	}
}

void KonamiCode::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	(void)render;
	(void)states;
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

#include "UnlockEasy.hpp"
#include "PostEffectLayer.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

#include <iostream>

UnlockEasy::UnlockEasy(void) :
	m_index(0u)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

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

UnlockEasy::~UnlockEasy(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void UnlockEasy::update(sf::Time frameTime)
{
	(void)frameTime;
	if (m_index == 10u)
	{
		m_index = 0u;
		octo::Application::getStateManager().change("game");
		Progress::getInstance().setEasyUnlocked(true);
	}
}

void UnlockEasy::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	(void)render;
	(void)states;
}

bool UnlockEasy::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::E:
			(m_index == 0u || m_index == 7u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::A:
			m_index == 1u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::S:
			m_index == 2u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::Y:
			m_index == 3u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::M:
			m_index == 4u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::O:
			(m_index == 5u || m_index == 8u) ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::D:
			m_index == 6u ? m_index++ : m_index = 0u;
			break;
		case sf::Keyboard::N:
			m_index == 9u ? m_index++ : m_index = 0u;
			break;
		default:
			m_index = 0u;
			break;
	}
	return true;
}

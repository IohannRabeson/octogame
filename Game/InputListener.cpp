#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <cassert>
#include "InputListener.hpp"

InputListener::InputListener(void) :
	m_joystickLT(false),
	m_joystickRT(false),
	m_joystickAxisX(false),
	m_joystickAxisY(false),
	m_triggerLimit(0.f)
{
	// Unix
	// Playstation
	//m_inputs = { OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::Up, OctoKeys::Down, OctoKeys::Right, OctoKeys::Use,
	//OctoKeys::Left, OctoKeys::GroundRight, OctoKeys::GroundLeft, OctoKeys::SlowFall, OctoKeys::SlowFall, OctoKeys::Jump};
	// XBox 360
#ifdef __linux__
	m_inputs = { OctoKeys::Jump, OctoKeys::Use, OctoKeys::SlowFall, OctoKeys::Infos, OctoKeys::GroundRight,
		OctoKeys::GroundLeft, OctoKeys::None, OctoKeys::Menu, OctoKeys::None, OctoKeys::None, OctoKeys::None };
	m_triggerLimit = 0.f;
#elif _WIN32
	m_inputs = { OctoKeys::Jump, OctoKeys::Use, OctoKeys::SlowFall, OctoKeys::Infos, OctoKeys::GroundRight,
		OctoKeys::GroundLeft, OctoKeys::None, OctoKeys::Menu, OctoKeys::None, OctoKeys::None, OctoKeys::None };
	m_triggerLimit = 50.f;
#else // __APPLE__
	m_inputs = { OctoKeys::Jump, OctoKeys::Use, OctoKeys::SlowFall, OctoKeys::Infos, OctoKeys::GroundRight, OctoKeys::GroundLeft, OctoKeys::None, OctoKeys::None, OctoKeys::Menu, OctoKeys::None,
		OctoKeys::None, OctoKeys::Up, OctoKeys::Down, OctoKeys::Left, OctoKeys::Right, OctoKeys::None,
		OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None };
	m_triggerLimit = 0.f;
#endif
}

bool	InputListener::onInputPressed(InputListener::OctoKeys const &)
{
	return true;
}

bool	InputListener::onInputReleased(InputListener::OctoKeys const &)
{
	return true;
}

void	InputListener::addInputListener(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addJoystickListener(this);
	graphics.addKeyboardListener(this);
}

void	InputListener::removeInputListener(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeJoystickListener(this);
	graphics.removeKeyboardListener(this);
}

bool	InputListener::onPressed(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			onInputPressed(OctoKeys::Left);
			break;
		case sf::Keyboard::Right:
			onInputPressed(OctoKeys::Right);
			break;
		case sf::Keyboard::Space:
			onInputPressed(OctoKeys::Jump);
			break;
		case sf::Keyboard::Up:
			onInputPressed(OctoKeys::Up);
			onInputPressed(OctoKeys::SlowFall);
			break;
		case sf::Keyboard::Down:
			onInputPressed(OctoKeys::Down);
			break;
		case sf::Keyboard::W:
			onInputPressed(OctoKeys::Use);
			break;
		case sf::Keyboard::A:
			onInputPressed(OctoKeys::GroundRight);
			break;
		case sf::Keyboard::D:
			onInputPressed(OctoKeys::GroundLeft);
			break;
		case sf::Keyboard::Return:
			onInputPressed(OctoKeys::SelectMenu);
			break;
		case sf::Keyboard::Escape:
			onInputPressed(OctoKeys::Menu);
			break;
		case sf::Keyboard::S:
			onInputPressed(OctoKeys::Infos);
			break;
		default:
			break;
	}
	return true;
}

bool	InputListener::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			onInputReleased(OctoKeys::Left);
			break;
		case sf::Keyboard::Right:
			onInputReleased(OctoKeys::Right);
			break;
		case sf::Keyboard::Space:
			onInputReleased(OctoKeys::Jump);
			break;
		case sf::Keyboard::Up:
			onInputReleased(OctoKeys::Up);
			onInputReleased(OctoKeys::SlowFall);
			break;
		case sf::Keyboard::Down:
			onInputReleased(OctoKeys::Down);
			break;
		case sf::Keyboard::W:
			onInputReleased(OctoKeys::Use);
			break;
		case sf::Keyboard::A:
			onInputReleased(OctoKeys::GroundRight);
			break;
		case sf::Keyboard::D:
			onInputReleased(OctoKeys::GroundLeft);
			break;
		case sf::Keyboard::Return:
			onInputReleased(OctoKeys::SelectMenu);
			break;
		case sf::Keyboard::Escape:
			onInputReleased(OctoKeys::Menu);
			break;
		case sf::Keyboard::S:
			onInputReleased(OctoKeys::Infos);
			break;
		default:
			break;
	}
	return true;
}

void	InputListener::onMoved(sf::Event::JoystickMoveEvent const& event)
{
	if (sf::Joystick::isConnected(0) && event.joystickId == 0)
	{
		if (event.axis == sf::Joystick::X || event.axis == sf::Joystick::PovX)
		{
			if (event.position > 50)
			{
				m_joystickAxisX = true;
				onInputPressed(OctoKeys::Right);
			}
			else if (event.position < -50)
			{
				m_joystickAxisX = true;
				onInputPressed(OctoKeys::Left);
			}
			else if (m_joystickAxisX == true)
			{
				m_joystickAxisX = false;
				onInputReleased(OctoKeys::Left);
				onInputReleased(OctoKeys::Right);
			}
		}
#ifdef _WIN32
		// If Xbox controller

		if (event.axis == sf::Joystick::Y)
		{
			if (event.position < -50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Up);
			}
			else if (event.position > 50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Down);
			}
			else if (m_joystickAxisY == true)
			{
				m_joystickAxisY = false;
				onInputReleased(OctoKeys::Up);
				onInputReleased(OctoKeys::Down);
			}
		}
		else if (event.axis == sf::Joystick::PovY)
		{
			if (event.position < -50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Down);
			}
			else if (event.position > 50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Up);
			}
			else if (m_joystickAxisY == true)
			{
				m_joystickAxisY = false;
				onInputReleased(OctoKeys::Up);
				onInputReleased(OctoKeys::Down);
			}
		}

		if (event.axis == sf::Joystick::Z)
		{
			if (event.position > 0.f) // LT
			{
				if (event.position > m_triggerLimit && !m_joystickLT)
				{
					m_joystickLT = true;
					onInputPressed(OctoKeys::GroundRight);
				}
				else if (event.position <= m_triggerLimit && m_joystickLT)
				{
					m_joystickLT = false;
					onInputReleased(OctoKeys::GroundRight);
				}
			}
			else //RT
			{
				if (event.position < -m_triggerLimit && !m_joystickRT)
				{
					m_joystickRT = true;
					//onInputPressed(OctoKeys::GroundLeft);
				}
				else if (event.position >= -m_triggerLimit && m_joystickRT)
				{
					m_joystickRT = false;
					//onInputReleased(OctoKeys::GroundLeft);
				}
			}
		}
#else
		if (event.axis == sf::Joystick::Y || event.axis == sf::Joystick::PovY)
		{
			if (event.position < -50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Up);
			}
			else if (event.position > 50)
			{
				m_joystickAxisY = true;
				onInputPressed(OctoKeys::Down);
			}
			else if (m_joystickAxisY == true)
			{
				m_joystickAxisY = false;
				onInputReleased(OctoKeys::Up);
				onInputReleased(OctoKeys::Down);
			}
		}

		if (event.axis == sf::Joystick::R) //LT
		{
			if (event.position > m_triggerLimit && !m_joystickLT)
			{
				m_joystickLT = true;
				//onInputPressed(OctoKeys::GroundLeft);
			}
			else if (event.position <= m_triggerLimit && m_joystickLT)
			{
				m_joystickLT = false;
				//onInputReleased(OctoKeys::GroundLeft);
			}
		}
		if (event.axis == sf::Joystick::Z) //RT
		{
			if (event.position > m_triggerLimit && !m_joystickRT)
			{
				m_joystickRT = true;
				onInputPressed(OctoKeys::GroundRight);
			}
			else if (event.position <= m_triggerLimit && m_joystickRT)
			{
				m_joystickRT = false;
				onInputReleased(OctoKeys::GroundRight);
			}
		}


#endif
	}
}

void	InputListener::onPressed(sf::Event::JoystickButtonEvent const& event)
{
	if (sf::Joystick::isConnected(0) && event.joystickId == 0)
	{
		assert(event.button < m_inputs.size());
		switch (m_inputs[event.button])
		{
			case OctoKeys::Left:
				onInputPressed(OctoKeys::Left);
				break;
			case OctoKeys::Right:
				onInputPressed(OctoKeys::Right);
				break;
			case OctoKeys::Jump:
				onInputPressed(OctoKeys::Jump);
				break;
			case OctoKeys::Up:
				onInputPressed(OctoKeys::Up);
				break;
			case OctoKeys::Down:
				onInputPressed(OctoKeys::Down);
				break;
			case OctoKeys::Use:
				onInputPressed(OctoKeys::Use);
				break;
			case OctoKeys::GroundRight:
				onInputPressed(OctoKeys::GroundRight);
				break;
			case OctoKeys::GroundLeft:
				onInputPressed(OctoKeys::GroundLeft);
				break;
			case OctoKeys::SlowFall:
				onInputPressed(OctoKeys::SlowFall);
				onInputPressed(OctoKeys::Up);
				break;
			case OctoKeys::Menu:
				onInputPressed(OctoKeys::Menu);
				break;
			case OctoKeys::Infos:
				onInputPressed(OctoKeys::Infos);
				break;
			default:
				break;
		}
	}
}

void	InputListener::onReleased(sf::Event::JoystickButtonEvent const& event)
{
	if (sf::Joystick::isConnected(0) && event.joystickId == 0)
	{
		assert(event.button < m_inputs.size());
		switch (m_inputs[event.button])
		{
			case OctoKeys::Left:
				onInputReleased(OctoKeys::Left);
				break;
			case OctoKeys::Right:
				onInputReleased(OctoKeys::Right);
				break;
			case OctoKeys::Jump:
				onInputReleased(OctoKeys::Jump);
				break;
			case OctoKeys::Up:
				onInputReleased(OctoKeys::Up);
				break;
			case OctoKeys::Down:
				onInputReleased(OctoKeys::Down);
				break;
			case OctoKeys::Use:
				onInputReleased(OctoKeys::Use);
				break;
			case OctoKeys::GroundRight:
				onInputReleased(OctoKeys::GroundRight);
				break;
			case OctoKeys::GroundLeft:
				onInputReleased(OctoKeys::GroundLeft);
				break;
			case OctoKeys::SelectMenu:
				onInputPressed(OctoKeys::SelectMenu);
				break;
			case OctoKeys::SlowFall:
				onInputReleased(OctoKeys::SlowFall);
				onInputReleased(OctoKeys::Up);
				break;
			case OctoKeys::Menu:
				onInputReleased(OctoKeys::Menu);
				break;
			case OctoKeys::Infos:
				onInputReleased(OctoKeys::Infos);
				break;
			default:
				break;
		}
	}
}

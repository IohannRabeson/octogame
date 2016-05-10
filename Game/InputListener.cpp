#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <cassert>
#include "InputListener.hpp"

InputListener::InputListener(void) :
	m_joystickLT(false),
	m_joystickRT(false),
	m_joystickAxisX(false),
	m_joystickAxisY(false)
{
	// Unix
	// Playstation
	//m_inputs = { OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::Up, OctoKeys::Down, OctoKeys::Right, OctoKeys::Use,
	//OctoKeys::Left, OctoKeys::GroundRight, OctoKeys::GroundLeft, OctoKeys::SlowFall, OctoKeys::SlowFall, OctoKeys::Space};
	// XBox 360
#ifdef __linux__
	m_inputs = { OctoKeys::Space, OctoKeys::Use, OctoKeys::None, OctoKeys::None, OctoKeys::SlowFall,
		OctoKeys::SlowFall, OctoKeys::None, OctoKeys::Return, OctoKeys::None, OctoKeys::None, OctoKeys::None };
#elif _WIN32
	m_inputs = { OctoKeys::Space, OctoKeys::Use, OctoKeys::None, OctoKeys::None, OctoKeys::SlowFall,
		OctoKeys::SlowFall, OctoKeys::None, OctoKeys::Return, OctoKeys::None, OctoKeys::None, OctoKeys::None };
#else // __APPLE__
	m_inputs = { OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::Return,
		OctoKeys::None, OctoKeys::None, OctoKeys::None, OctoKeys::SlowFall, OctoKeys::SlowFall, OctoKeys::None,
		OctoKeys::Space, OctoKeys::Use, OctoKeys::None, OctoKeys::None };
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
			onInputPressed(OctoKeys::Space);
			break;
		case sf::Keyboard::Up:
			onInputPressed(OctoKeys::Up);
			onInputPressed(OctoKeys::SlowFall);
			break;
		case sf::Keyboard::Down:
			onInputPressed(OctoKeys::Down);
			break;
		case sf::Keyboard::E:
			onInputPressed(OctoKeys::Use);
			break;
		case sf::Keyboard::S:
			onInputPressed(OctoKeys::GroundRight);
			break;
		case sf::Keyboard::F:
			onInputPressed(OctoKeys::GroundLeft);
			break;
		case sf::Keyboard::Return:
			onInputPressed(OctoKeys::Return);
			break;
		case sf::Keyboard::Escape:
			onInputPressed(OctoKeys::Escape);
			break;
		case sf::Keyboard::D:
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
			onInputReleased(OctoKeys::Space);
			break;
		case sf::Keyboard::Up:
			onInputReleased(OctoKeys::Up);
			break;
		case sf::Keyboard::Down:
			onInputReleased(OctoKeys::Down);
			break;
		case sf::Keyboard::E:
			onInputReleased(OctoKeys::Use);
			break;
		case sf::Keyboard::S:
			onInputReleased(OctoKeys::GroundRight);
			break;
		case sf::Keyboard::F:
			onInputReleased(OctoKeys::GroundLeft);
			break;
		case sf::Keyboard::Return:
			onInputReleased(OctoKeys::Return);
			break;
		case sf::Keyboard::Escape:
			onInputReleased(OctoKeys::Escape);
			break;
		case sf::Keyboard::D:
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
		// If Xbox controller
		if (event.axis == sf::Joystick::Z) //LT
		{
			if (event.position > 0.f && !m_joystickLT)
			{
				m_joystickLT = true;
				onInputPressed(OctoKeys::GroundLeft);
			}
			else if (event.position <= 0.f && m_joystickLT)
			{
				m_joystickLT = false;
				onInputReleased(OctoKeys::GroundLeft);
			}
		}
		if (event.axis == sf::Joystick::R) //RT
		{
			if (event.position > 0.f && !m_joystickRT)
			{
				m_joystickRT = true;
				onInputPressed(OctoKeys::GroundRight);
			}
			else if (event.position <= 0.f && m_joystickRT)
			{
				m_joystickRT = false;
				onInputReleased(OctoKeys::GroundRight);
			}
		}

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
			case OctoKeys::Space:
				onInputPressed(OctoKeys::Space);
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
			case OctoKeys::Escape:
				onInputPressed(OctoKeys::Escape);
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
			case OctoKeys::Space:
				onInputReleased(OctoKeys::Space);
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
			case OctoKeys::SlowFall:
				onInputReleased(OctoKeys::SlowFall);
				onInputReleased(OctoKeys::Up);
				break;
			case OctoKeys::Escape:
				onInputReleased(OctoKeys::Escape);
				break;
			default:
				break;
		}
	}
}

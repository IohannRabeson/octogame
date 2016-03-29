#include <Application.hpp>
#include <GraphicsManager.hpp>
#include "InputListener.hpp"

#include <iostream>

InputListener::InputListener(void)
{
}

InputListener::~InputListener(void)
{
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
	if (sf::Joystick::isConnected(0))
		graphics.addJoystickListener(this);
	else
		graphics.addKeyboardListener(this);
}

void	InputListener::removeInputListener(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	if (sf::Joystick::isConnected(0))
		graphics.removeJoystickListener(this);
	else
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
		default:
			break;
	}
	return true;
}


void	InputListener::onMoved(sf::Event::JoystickMoveEvent const& event)
{
	if (sf::Joystick::isConnected(0) && event.joystickId == 0)
	{
		if (event.axis == sf::Joystick::X)
		{
			if (event.position > 50)
			{
				m_joystickBoolX = true;
				onInputPressed(OctoKeys::Right);
			}
			else if (event.position < -50)
			{
				m_joystickBoolX = true;
				onInputPressed(OctoKeys::Left);
			}
			else if (m_joystickBoolX == true)
			{
				m_joystickBoolX = false;
				onInputReleased(OctoKeys::Left);
				onInputReleased(OctoKeys::Right);
			}
		}

		if (event.axis == sf::Joystick::Y)
		{
			if (event.position < -50)
			{
				m_joystickBoolY = true;
				//onInputPressed(OctoKeys::Up);
			}
			else if (event.position > 50)
			{
				m_joystickBoolY = true;
				onInputPressed(OctoKeys::Down);
			}
			else if (m_joystickBoolY == true)
			{
				m_joystickBoolY = false;
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
		switch (event.button)
		{
			case 7:
				onInputPressed(OctoKeys::Left);
				break;
			case 5:
				onInputPressed(OctoKeys::Right);
				break;
			case 14:
				onInputPressed(OctoKeys::Space);
				break;
			case 4:
				onInputPressed(OctoKeys::Up);
				break;
			case 6:
				onInputPressed(OctoKeys::Down);
				break;
			case 13:
				onInputPressed(OctoKeys::Use);
				break;
			case 9:
				onInputPressed(OctoKeys::GroundRight);
				break;
			case 8:
				onInputPressed(OctoKeys::GroundLeft);
				break;
			case 10:
			case 11:
				onInputPressed(OctoKeys::SlowFall);
				onInputPressed(OctoKeys::Up);
				break;
			case 3:
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
		switch (event.button)
		{
			case 7:
				onInputReleased(OctoKeys::Left);
				break;
			case 5:
				onInputReleased(OctoKeys::Right);
				break;
			case 14:
				onInputReleased(OctoKeys::Space);
				break;
			case 4:
				onInputReleased(OctoKeys::Up);
				break;
			case 6:
				onInputReleased(OctoKeys::Down);
				break;
			case 13:
				onInputReleased(OctoKeys::Use);
				break;
			case 9:
				onInputReleased(OctoKeys::GroundRight);
				break;
			case 8:
				onInputReleased(OctoKeys::GroundLeft);
				break;
			case 10:
			case 11:
				onInputReleased(OctoKeys::SlowFall);
				onInputReleased(OctoKeys::Up);
				break;
			case 3:
				onInputReleased(OctoKeys::Escape);
				break;
			default:
				break;
		}
	}
}


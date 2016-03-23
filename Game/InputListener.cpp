#include <Application.hpp>
#include <GraphicsManager.hpp>
#include "InputListener.hpp"

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
	graphics.addKeyboardListener(this);
}

void	InputListener::removeInputListener(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
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
	(void)event;
	if (sf::Joystick::isConnected(0))
	{
	}
}
void	InputListener::onPressed(sf::Event::JoystickButtonEvent const& event)
{
	(void)event;
	if (sf::Joystick::isConnected(0))
	{
	}
}

void	InputListener::onReleased(sf::Event::JoystickButtonEvent const& event)
{
	(void)event;
	if (sf::Joystick::isConnected(0))
	{
	}
}


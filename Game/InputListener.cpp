#include <Application.hpp>
#include <GraphicsManager.hpp>
#include "InputListener.hpp"

InputListener::InputListener(void)
{
}

InputListener::~InputListener(void)
{
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
		default:
			break;
	}
	return true;
}

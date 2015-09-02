#include "ControlMenu.hpp"
#include <Application.hpp>

ControlMenu::ControlMenu(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

void ControlMenu::setup(void)
{
	// Set main menu
	std::vector<std::string> subMenuTitle;
	subMenuTitle.resize(3);
	subMenuTitle[0] = "Left/Right : move";
	subMenuTitle[1] = "Space : jump, double jump";
	subMenuTitle[2] = "Up : slow fall, climb elevators";
	m_bubble.setup(subMenuTitle, sf::Color(255, 255, 255, 255), 20u);
	m_bubble.setType(ABubble::Type::Left);
}

void ControlMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	m_bubble.setPosition(position);
	m_bubble.update(frameTime);
	m_bubble.setActive(isActive());
	m_isKeyboardListening = true;
}

void ControlMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_bubble, states);
}

bool ControlMenu::onPressed(sf::Event::KeyEvent const &event)
{
	if (m_isKeyboardListening)
	{
		switch (event.code)
		{
			case sf::Keyboard::Left:
			{
				setActive(false);
				break;
			}
			case sf::Keyboard::Return:
			{
				setActive(false);
				break;
			}
			default:
				break;
		}
	}
	return (true);
}

#include "AMenuSelection.hpp"
#include <Application.hpp>
#include <assert.h>

AMenuSelection::AMenuSelection(void) :
	m_type(ABubble::Type::Left),
	m_characterSize(20u),
	m_indexCursor(0),
	m_isKeyboardListening(false)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

std::size_t AMenuSelection::addMenu(std::string const & name, AMenu * menu)
{
	m_menuTitles.push_back(name);
	m_menus.push_back(menu);
	return (m_menus.size() - 1u);
}

void AMenuSelection::setup(void)
{
	createMenus();
	assert(m_menus.size() > 0);
	m_bubble.setup(m_menuTitles, sf::Color(255, 255, 255, 255), m_characterSize);
	m_bubble.setType(m_type);

	for (std::size_t i = 0; i < m_menus.size(); i++)
		m_menus[i]->setup();

	m_cursorPosition = m_bubble.getCursorPosition();
}

void AMenuSelection::update(sf::Time frameTime, sf::Vector2f const & position)
{
	m_bubble.setPosition(position);
	m_bubble.update(frameTime);
	m_bubble.setActive(isActive());

	sf::Vector2f const & contentPosition = m_bubble.getContentUpLeft();
	m_isKeyboardListening = true;
	for (std::size_t i = 0; i < m_menus.size(); i++)
	{
		if (m_indexCursor == i)
			m_menus[i]->update(frameTime, m_cursorPosition[i] + contentPosition);
		if (m_menus[i]->isActive())
			m_isKeyboardListening = false;
	}
}


void AMenuSelection::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_bubble, states);
	for (std::size_t i = 0; i < m_menus.size(); i++)
	{
		if (m_indexCursor == i)
			render.draw(*m_menus[i]);
	}
}


bool AMenuSelection::onPressed(sf::Event::KeyEvent const &event)
{
	if (m_isKeyboardListening)
	{
		switch (event.code)
		{
			case sf::Keyboard::Up:
			{
				if (m_indexCursor == 0u)
					m_indexCursor = m_menus.size() - 1;
				else
					m_indexCursor -= 1;
				break;
			}
			case sf::Keyboard::Down:
			{
				if (m_indexCursor >= m_menus.size() - 1)
					m_indexCursor = 0u;
				else
					m_indexCursor += 1;
				break;
			}
			case sf::Keyboard::Right:
			{
				onSelection();
				break;
			}
			case sf::Keyboard::Return:
			{
				onSelection();
				break;
			}
			case sf::Keyboard::Left:
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

void AMenuSelection::onSelection(void)
{
	for (std::size_t i = 0; i < m_menus.size(); i++)
	{
		if (m_indexCursor == i)
			m_menus[i]->setActive(true);
	}
	m_isKeyboardListening = false;
}

void AMenuSelection::setCharacterSize(std::size_t size)
{
	m_characterSize = size;
}

void AMenuSelection::setBubbleType(ABubble::Type type)
{
	m_type = type;
}

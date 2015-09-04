#include "AMenuSelection.hpp"
#include <Application.hpp>
#include <assert.h>

AMenuSelection::AMenuSelection(void) :
	m_type(ABubble::Type::Left),
	m_characterSize(20u),
	m_indexCursor(0),
	m_isKeyboard(false)
{
}

void AMenuSelection::addMenu(std::string const & name, AMenu * menu)
{
	m_menuTitles.push_back(name);
	if (menu)
		m_menus.push_back(menu);
}

void AMenuSelection::setup(void)
{
	createMenus();
	m_bubble.setup(m_menuTitles, sf::Color(255, 255, 255, 255), m_characterSize);
	m_bubble.setType(ABubble::None);

	for (std::size_t i = 0; i < m_menus.size(); i++)
		m_menus[i]->setup(this);

	m_cursor.setRadius(10);
	m_cursor.setPointCount(8);
	m_cursor.setOrigin(10, 10);
	//TODO: Investigate this, should be 360.f / 8.f
	m_cursor.setRotation(360.f / 16.f);
	m_cursorPosition = m_bubble.getCursorPosition();
}

void AMenuSelection::setKeyboard(bool isKeyboard)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	if (m_isKeyboard == false && isKeyboard == true)
	{
		graphics.addKeyboardListener(this);
		m_isKeyboard = true;
	}
	else if (m_isKeyboard == true && isKeyboard == false)
	{
		graphics.removeKeyboardListener(this);
		m_isKeyboard = false;
	}
}

void AMenuSelection::update(sf::Time frameTime, sf::Vector2f const & position)
{
	sf::Vector2f const & contentPosition = m_bubble.getContentUpLeft();
	for (std::size_t i = 0; i < m_menus.size(); i++)
		m_menus[i]->update(frameTime, m_cursorPosition[i] + contentPosition);

	if (getState() == AMenu::State::Active)
	{
		m_bubble.setType(m_type);
		m_bubble.setPosition(position);
		m_bubble.setActive(true);
		m_bubble.update(frameTime);
		m_cursor.setPosition(m_cursorPosition[m_indexCursor] + contentPosition);
		setKeyboard(true);
	}
	else
		setKeyboard(false);
}


void AMenuSelection::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (std::size_t i = 0; i < m_menus.size(); i++)
		render.draw(*m_menus[i]);
	AMenu::State const & state = getState();
	if (state == AMenu::State::Active || state == AMenu::State::Draw)
		render.draw(m_bubble, states);
	if (state == AMenu::State::Active && m_menus.size())
		render.draw(m_cursor);
}

bool AMenuSelection::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
		{
			setState(AMenu::State::Hide);
			AMenu * backMenu = getBackMenu();
			if (backMenu)
				backMenu->setState(AMenu::State::Active);
			break;
		}
		default:
			break;
	}

	if (m_menus.size())
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
			m_menus[i]->setState(AMenu::State::Active);
	}
	setState(AMenu::State::Draw);
}

void AMenuSelection::setCharacterSize(std::size_t size)
{
	m_characterSize = size;
}

void AMenuSelection::setBubbleType(ABubble::Type type)
{
	m_type = type;
}

void AMenuSelection::setIndexCursor(std::size_t index)
{
	if (index < m_menus.size())
		m_indexCursor = index;
}

std::size_t AMenuSelection::getIndexCursor(void)
{
	return m_indexCursor;
}


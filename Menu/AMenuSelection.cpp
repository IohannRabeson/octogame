#include "AMenuSelection.hpp"
#include "ResourceDefinitions.hpp"
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <assert.h>

AMenuSelection::AMenuSelection(void) :
	m_type(ABubble::Type::Right),
	m_characterSize(20u),
	m_indexCursor(0u),
	m_indexSave(0u),
	m_isKeyboard(false)
{
}

AMenuSelection::~AMenuSelection(void)
{
	setKeyboard(false);
}

void AMenuSelection::addMenu(std::wstring const & name, std::unique_ptr<AMenu>&& menu)
{
	m_menuTitles.push_back(name);
	if (menu)
		m_menus.push_back(std::move(menu));
}

void AMenuSelection::setup(void)
{
	createMenus();
	setupBubble();
}

void AMenuSelection::setupBubble(void)
{
	m_bubble.setup(m_menuTitles, sf::Color(255, 255, 255, 150), m_characterSize);
	m_bubble.setType(ABubble::None);

	//create octogon for cursor
	m_cursor.setRadius(10);
	m_cursor.setPointCount(8);
	m_cursor.setOrigin(10, 10);
	m_cursor.setRotation(360.f / 16.f);
	m_cursor.setFillColor(sf::Color(255, 255, 255, 150));
	m_cursorPosition = m_bubble.getCursorPosition();

	for (std::size_t i = 0; i < m_menus.size(); i++)
		m_menus[i]->setup(this);
}

void AMenuSelection::setKeyboard(bool isKeyboard)
{
	if (m_isKeyboard == false && isKeyboard == true)
	{
		InputListener::addInputListener();
		m_isKeyboard = true;
	}
	else if (m_isKeyboard == true && isKeyboard == false)
	{
		InputListener::removeInputListener();
		m_isKeyboard = false;
	}
}

void AMenuSelection::update(sf::Time frameTime, sf::Vector2f const & position)
{
	m_bubble.setPosition(position);
	//TODO: Find better way to do that
	m_bubble.setIndexCursor(m_indexCursor);
	m_bubble.update(frameTime);
	sf::Vector2f const & contentPosition = m_bubble.getContentUpLeft();
	for (std::size_t i = 0; i < m_menus.size(); i++)
		m_menus[i]->update(frameTime, m_cursorPosition[i] + contentPosition);

	if (getState() == AMenu::State::Active)
	{
		setKeyboard(true);
		m_bubble.setActive(true);
		m_bubble.setType(m_type);
		m_cursor.setPosition(m_cursorPosition[m_indexCursor] + contentPosition);
	}
	else
	{
		m_indexSave = m_indexCursor;
		setKeyboard(false);
	}
}


void AMenuSelection::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	for (std::size_t i = 0; i < m_menus.size(); i++)
		render.draw(*m_menus[i]);
	AMenu::State const & state = getState();
	if (state == AMenu::State::Active || state == AMenu::State::Draw)
	{
		render.draw(m_bubble, states);
		if (state == AMenu::State::Active && m_menus.size())
			render.draw(m_cursor);
	}
}

bool AMenuSelection::onInputPressed(InputListener::OctoKeys const & key)
{
	if (!m_input)
	{
		octo::AudioManager &		audio = octo::Application::getAudioManager();
		octo::ResourceManager &		resources = octo::Application::getResourceManager();
		audio.playSound(resources.getSound(MENU_SOUND_OGG), 0.15f);

		m_input = true;
		switch (key)
		{
			case OctoKeys::Left:
			case OctoKeys::Use:
			case OctoKeys::Menu:
			{
				setState(AMenu::State::Hide);
				AMenu * backMenu = getBackMenu();
				if (backMenu)
					backMenu->setState(AMenu::State::Active);
				m_indexCursor = m_indexSave;
				break;
			}
			default:
				break;
		}
	
		if (m_menus.size())
		{
			switch (key)
			{
				case OctoKeys::Up:
				{
					if (m_indexCursor == 0u)
						m_indexCursor = m_menus.size() - 1;
					else
						m_indexCursor -= 1;
					break;
				}
				case OctoKeys::Down:
				{
					if (m_indexCursor >= m_menus.size() - 1)
						m_indexCursor = 0u;
					else
						m_indexCursor += 1;
					break;
				}
				case OctoKeys::Jump:
				case OctoKeys::Right:
				case OctoKeys::SelectMenu:
				{
					onSelection();
					break;
				}
				default:
					break;
			}
		}
	}
	return (true);
}

bool AMenuSelection::onInputReleased(InputListener::OctoKeys const &)
{
	m_input = false;
	return true;
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

std::size_t AMenuSelection::getIndexCursor(void) const
{
	return m_indexCursor;
}

void AMenuSelection::setIsFontSelect(bool isFontSelect)
{
	m_bubble.setIsFontSelect(isFontSelect);
}

#include "AMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <sstream>

AMenu::AMenu(void) :
	m_currentState(State::Hide),
	m_backMenu(nullptr),
	m_missingText(L"Missing menu")
{
}

void	AMenu::setup(AMenu * menu)
{
	m_backMenu = menu;

	setup();
}

void AMenu::setState(State state)
{
	m_currentState = state;
}

AMenu::State AMenu::getState(void) const
{
	return m_currentState;
}

AMenu * AMenu::getBackMenu(void)
{
	return m_backMenu;
}

std::wstring const & AMenu::getText(std::string const & text)
{
	std::vector<std::wstring> const & menuText = TextManager::getInstance().getTexts(text);
	if (menuText.size())
		return menuText[0];
	return m_missingText;
}

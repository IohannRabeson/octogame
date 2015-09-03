#include "AMenu.hpp"

AMenu::AMenu(void) :
	m_currentState(State::Hide),
	m_backMenu(nullptr)
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

void AMenu::setBackMenuActive(void)
{
	if (m_backMenu)
		setState(State::Active);
}

AMenu * AMenu::getBackMenu(void)
{
	return m_backMenu;
}

#include "AMenu.hpp"

AMenu::AMenu(void) :
	m_isActive(false)
{
}

bool AMenu::isActive(void) const
{
	return m_isActive;
}

void AMenu::setActive(bool isActive)
{
	m_isActive = isActive;
}

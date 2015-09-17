#include "ControlMenu.hpp"
#include "Progress.hpp"

ControlMenu::ControlMenu(void) :
	m_isCreatedMoveMap(false),
	m_isCreatedRepair(false),
	m_isCreatedJump(false),
	m_isCreatedDoubleJump(false),
	m_isCreatedSlowFall(false)
{
}

void ControlMenu::createMenus(void)
{
	addMenu("Deplacement : Left/Right", nullptr);
	addMenu("Utiliser : Up", nullptr);
}

void ControlMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	Progress & progress = Progress::getInstance();

	if (progress.canMoveMap() && m_isCreatedMoveMap == false)
	{
		addMenu("Controle du sol : S/F", nullptr);
		m_isCreatedMoveMap = true;
		setupBubble();
	}
	if (progress.canRepair() && m_isCreatedRepair == false)
	{
		addMenu("Reparation : E", nullptr);
		m_isCreatedRepair = true;
		setupBubble();
	}
	if (progress.canJump() && m_isCreatedJump == false)
	{
		addMenu("Jump : Space", nullptr);
		m_isCreatedJump = true;
		setupBubble();
	}
	if (progress.canDoubleJump() && m_isCreatedDoubleJump == false)
	{
		addMenu("Double Jump : Space", nullptr);
		m_isCreatedDoubleJump = true;
		setupBubble();
	}
	if (progress.canSlowFall() && m_isCreatedSlowFall == false)
	{
		addMenu("Chutte lente : Up", nullptr);
		m_isCreatedSlowFall = true;
		setupBubble();
	}
	AMenuSelection::update(frameTime, position);
}

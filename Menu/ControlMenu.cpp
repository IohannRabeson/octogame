#include "ControlMenu.hpp"
#include "Progress.hpp"

ControlMenu::ControlMenu(void) :
	m_isCreatedMoveMap(false),
	m_isCreatedRepair(false),
	m_isCreatedJump(false),
	m_isCreatedDoubleJump(false),
	m_isCreatedWaterJump(false),
	m_isCreatedSlowFall(false)
{
}

void ControlMenu::createMenus(void)
{
	addMenu(L"Déplacement : [GAUCHE]/[DROITE]", nullptr);
}

void ControlMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	Progress & progress = Progress::getInstance();

	if (progress.canMoveMap() && m_isCreatedMoveMap == false)
	{
		addMenu(L"Contrôle du sol : [S]/[F]", nullptr);
		addMenu(L"Portail : [E]", nullptr);
		m_isCreatedMoveMap = true;
		setupBubble();
	}
	if (progress.canRepair() && m_isCreatedRepair == false)
	{
		addMenu(L"Elevator : [E]", nullptr);
		m_isCreatedRepair = true;
		setupBubble();
	}
	if (progress.canJump() && m_isCreatedJump == false)
	{
		addMenu(L"Saut : [ESPACE]", nullptr);
		m_isCreatedJump = true;
		setupBubble();
	}
	if (progress.canDoubleJump() && m_isCreatedDoubleJump == false)
	{
		addMenu(L"Double Saut : [ESPACE]", nullptr);
		m_isCreatedDoubleJump = true;
		setupBubble();
	}
	if (progress.canUseWaterJump() && m_isCreatedWaterJump == false)
	{
		addMenu(L"Water Jump : [HAUT]", nullptr);
		m_isCreatedWaterJump = true;
		setupBubble();
	}
	if (progress.canSlowFall() && m_isCreatedSlowFall == false)
	{
		addMenu(L"Chutte lente : [HAUT]", nullptr);
		m_isCreatedSlowFall = true;
		setupBubble();
	}
	AMenuSelection::update(frameTime, position);
}

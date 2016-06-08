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
	addMenu(AMenu::getText("control_move"), nullptr);
	setIsFontSelect(false);
}

void ControlMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	Progress & progress = Progress::getInstance();

	if (progress.canMoveMap() && m_isCreatedMoveMap == false)
	{
		addMenu(AMenu::getText("control_ground"), nullptr);
		addMenu(AMenu::getText("control_portal"), nullptr);
		m_isCreatedMoveMap = true;
		setupBubble();
	}
	if (progress.canRepair() && m_isCreatedRepair == false)
	{
		addMenu(AMenu::getText("control_elevator"), nullptr);
		m_isCreatedRepair = true;
		setupBubble();
	}
	if (progress.canJump() && m_isCreatedJump == false)
	{
		addMenu(AMenu::getText("control_jump"), nullptr);
		m_isCreatedJump = true;
		setupBubble();
	}
	if (progress.canDoubleJump() && m_isCreatedDoubleJump == false)
	{
		addMenu(AMenu::getText("control_double_jump"), nullptr);
		m_isCreatedDoubleJump = true;
		setupBubble();
	}
	if (progress.canUseWaterJump() && m_isCreatedWaterJump == false)
	{
		addMenu(AMenu::getText("control_water_jump"), nullptr);
		m_isCreatedWaterJump = true;
		setupBubble();
	}
	if (progress.canSlowFall() && m_isCreatedSlowFall == false)
	{
		addMenu(AMenu::getText("control_slow_fall"), nullptr);
		m_isCreatedSlowFall = true;
		setupBubble();
	}
	AMenuSelection::update(frameTime, position);
}

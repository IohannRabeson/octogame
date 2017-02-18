#include "ControlMenu.hpp"
#include "Progress.hpp"

ControlMenu::ControlMenu(void) :
	m_isCreatedMoveMap(false),
	m_isCreatedRepair(false),
	m_isCreatedJump(false),
	m_isCreatedDoubleJump(false),
	m_isCreatedSlowFall(false),
	m_isCreatedBalle(false)
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
		addMenu(AMenu::getText("control_camera"), nullptr);
		addMenu(AMenu::getText("control_camera_zoom"), nullptr);
		addMenu(AMenu::getText("control_portal"), nullptr);
		m_isCreatedMoveMap = true;
		setupBubble();
	}
	if (progress.canRepair() && m_isCreatedRepair == false)
	{
		addMenu(AMenu::getText("control_repair"), nullptr);
		addMenu(AMenu::getText("control_propulsion"), nullptr);
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
	if (progress.canSlowFall() && m_isCreatedSlowFall == false)
	{
		addMenu(AMenu::getText("control_slow_fall"), nullptr);
		m_isCreatedSlowFall = true;
		setupBubble();
	}
	if (progress.canUseBalle() && m_isCreatedBalle == false)
	{
		addMenu(AMenu::getText("control_balle"), nullptr);
		m_isCreatedBalle = true;
		setupBubble();
	}
	AMenuSelection::update(frameTime, position);
}

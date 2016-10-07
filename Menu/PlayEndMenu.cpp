#include "PlayEndMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

PlayEndMenu::PlayEndMenu(void)
{
	m_names.push_back(AMenu::getText("menu_play_current"));
	m_names.push_back(AMenu::getText("menu_play_ice"));
	m_names.push_back(AMenu::getText("menu_play_desert"));
	m_names.push_back(AMenu::getText("menu_play_jungle"));
	m_names.push_back(AMenu::getText("menu_play_water"));
	m_names.push_back(AMenu::getText("menu_play_final"));
}

void PlayEndMenu::createMenus(void)
{
	for (std::size_t i = 0; i < m_names.size(); i++)
		addMenu(m_names[i], std::unique_ptr<EmptyMenu>(new EmptyMenu()));
}

void PlayEndMenu::onSelection(void)
{
	octo::StateManager &		states = octo::Application::getStateManager();
	Progress &					progress = Progress::getInstance();

	switch (getIndexCursor())
	{
		case 0u:
			break;
		case 1u:
			progress.setNextDestination(Level::IceA);
			progress.setRespawnType(Progress::RespawnType::Portal);
			break;
		case 2u:
			progress.setNextDestination(Level::DesertA);
			progress.setRespawnType(Progress::RespawnType::Portal);
			break;
		case 3u:
			progress.setNextDestination(Level::JungleA);
			progress.setRespawnType(Progress::RespawnType::Portal);
			break;
		case 4u:
			progress.setNextDestination(Level::WaterA);
			progress.setRespawnType(Progress::RespawnType::Portal);
			break;
		case 5u:
			progress.setNextDestination(Level::Final);
			progress.setRespawnType(Progress::RespawnType::Portal);
			break;
		default:
			break;
	}
	states.change("transitionLevel");

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


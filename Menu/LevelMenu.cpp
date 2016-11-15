#include "LevelMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

LevelMenu::LevelMenu(void) :
	m_index(0u)
{
	m_names.push_back(L"Ice A");
	m_names.push_back(L"Ice B");
	m_names.push_back(L"Ice C");
	m_names.push_back(L"Ice D");
	m_names.push_back(L"Desert A");
	m_names.push_back(L"Desert B");
	m_names.push_back(L"Desert C");
	m_names.push_back(L"Desert D");
	m_names.push_back(L"Jungle A");
	m_names.push_back(L"Jungle B");
	m_names.push_back(L"Jungle C");
	m_names.push_back(L"Jungle D");
	m_names.push_back(L"Water A");
	m_names.push_back(L"Water B");
	m_names.push_back(L"Water C");
	m_names.push_back(L"Water D");
	m_names.push_back(L"Final");
	m_names.push_back(L"Red");
	m_names.push_back(L"Blue");
	m_names.push_back(L"Portal");
	m_names.push_back(L"Random");
	m_names.push_back(L"Rewards");
	m_names.push_back(L"Segfault");
	m_names.push_back(L"Uknown");
	m_names.push_back(L"Uknown");
	m_names.push_back(L"Uknown");
	m_names.push_back(L"Uknown");
	m_names.push_back(L"Uknown");
}

void LevelMenu::createMenus(void)
{
	std::vector<Level> const & levels = Progress::getInstance().getRegisteredLevels();
	Level const & level = Progress::getInstance().getInstance().getNextDestination();
	for (std::size_t i = 0; i < levels.size(); i++)
	{
		if (level == levels[i])
			m_index = i;
		addMenu(m_names[i], std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	}

	setIndexCursor(static_cast<std::size_t>(m_index));
}

void LevelMenu::onSelection(void)
{
	octo::StateManager &		states = octo::Application::getStateManager();
	std::vector<Level> const &	levels = Progress::getInstance().getRegisteredLevels();

	Progress::getInstance().setNextDestination(levels[getIndexCursor()]);
	Progress::getInstance().setRespawnType(Progress::RespawnType::Portal);
	states.change("game");

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


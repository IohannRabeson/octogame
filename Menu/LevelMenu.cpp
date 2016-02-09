#include "LevelMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

LevelMenu::LevelMenu(void):
	m_index(0u)
{
}

void LevelMenu::createMenus(void)
{
	std::vector<Level> const & levels = Progress::getInstance().getRegisteredLevels();
	Level level = Progress::getInstance().getInstance().getNextDestination();
	for (std::size_t i = 0; i < levels.size(); i++)
	{
		if (level == levels[i])
			m_index = i;
		addMenu(std::to_wstring((std::size_t)i), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	}

	setIndexCursor(static_cast<std::size_t>(m_index));
}

void LevelMenu::onSelection(void)
{
	std::vector<Level> const & levels = Progress::getInstance().getRegisteredLevels();
	Progress::getInstance().setNextDestination(levels[getIndexCursor()]);

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


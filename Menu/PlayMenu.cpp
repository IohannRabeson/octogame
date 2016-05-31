#include "PlayMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

PlayMenu::PlayMenu(void) :
	m_index(0u)
{
	if (!Progress::getInstance().isDemo())
	{
		m_names.push_back(L"Ice A");
		m_names.push_back(L"Ice B");
		m_names.push_back(L"Ice C");
		m_names.push_back(L"Ice D");
		m_names.push_back(L"Desert A");
		m_names.push_back(L"Desert B");
		m_names.push_back(L"Desert C");
		m_names.push_back(L"Jungle A");
		m_names.push_back(L"Jungle C");
		m_names.push_back(L"Water A");
		m_names.push_back(L"Water B");
		m_names.push_back(L"Random");
		m_names.push_back(L"Rewards");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
	}
	else
	{
		m_names.push_back(L"Ice A [DEMO]");
		m_names.push_back(L"Ice B [DEMO]");
		m_names.push_back(L"Ice C [DEMO]");
		m_names.push_back(L"Ice D [DEMO]");
		m_names.push_back(L"Desert A [DEMO]");
		m_names.push_back(L"Jungle A [DEMO]");
		m_names.push_back(L"Water A [DEMO]");
		m_names.push_back(L"Random [DEMO]");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
		m_names.push_back(L"Uknown");
	}
}

void PlayMenu::createMenus(void)
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

void PlayMenu::onSelection(void)
{
	std::vector<Level> const & levels = Progress::getInstance().getRegisteredLevels();
	Progress::getInstance().setNextDestination(levels[getIndexCursor()]);

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

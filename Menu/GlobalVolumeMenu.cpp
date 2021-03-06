#include "GlobalVolumeMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <SFML/Audio/Listener.hpp>
#include <Application.hpp>
#include <AudioManager.hpp>

GlobalVolumeMenu::GlobalVolumeMenu(void) :
	m_volume(1.f)
{
}

void GlobalVolumeMenu::createMenus(void)
{
	addMenu(L"0", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"1", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"2", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"3", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"4", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"5", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"6", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"7", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"8", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"9", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	m_volume = static_cast<std::size_t>(sf::Listener::getGlobalVolume());
	if (m_volume >= 100.f)
		m_volume = 99.f;
	setIndexCursor(static_cast<std::size_t>(m_volume / 10.f));
}

void GlobalVolumeMenu::onSelection(void)
{
	sf::Listener::setGlobalVolume(getIndexCursor() * 10);
	Progress::getInstance().setGlobalVolume(getIndexCursor() * 10);

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


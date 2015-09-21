#include "SoundVolumeMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

SoundVolumeMenu::SoundVolumeMenu(void)
{
}

void SoundVolumeMenu::createMenus(void)
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
	setIndexCursor(9u);
}

void SoundVolumeMenu::onSelection(void)
{
	octo::AudioManager & audio = octo::Application::getAudioManager();

	audio.setSoundVolume(getIndexCursor() * 10);

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


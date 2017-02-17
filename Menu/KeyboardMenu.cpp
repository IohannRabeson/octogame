#include "KeyboardMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

KeyboardMenu::KeyboardMenu(void)
{
}

void KeyboardMenu::createMenus(void)
{
	addMenu(L"qwerty", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"azerty", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getKeyboard()));
}

void KeyboardMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setKeyboard(static_cast<Progress::Keyboard>(getIndexCursor()));
	progress.save();
	TextManager::getInstance().loadTexts();

	if (progress.changeLevel() == false)
	{
		octo::StateManager & state = octo::Application::getStateManager();
		state.change("menu");
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}


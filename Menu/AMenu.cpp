#include "AMenu.hpp"
#include "Progress.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <sstream>

AMenu::AMenu(void) :
	m_currentState(State::Hide),
	m_backMenu(nullptr)
{
}

void	AMenu::setup(AMenu * menu)
{
	m_backMenu = menu;

	setup();
}

void AMenu::setState(State state)
{
	m_currentState = state;
}

AMenu::State AMenu::getState(void) const
{
	return m_currentState;
}

AMenu * AMenu::getBackMenu(void)
{
	return m_backMenu;
}

void AMenu::initTexts(void)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	Progress & progress = Progress::getInstance();

	std::wstringstream f(resources.getText(progress.getTextFile()).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		m_menuTexts[key] = line;
	}
}

std::wstring AMenu::getText(std::string const & text)
{
	if (m_menuTexts.find(text) != m_menuTexts.end())
		return m_menuTexts[text];
	return L"Missing menu";
}

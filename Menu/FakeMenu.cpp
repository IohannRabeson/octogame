#include "FakeMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"

FakeMenu::FakeMenu(void) :
	m_quit(false)
{
}

void FakeMenu::createMenus(void)
{
	addMenu(AMenu::getText("menu_quit"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("menu_restart"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("menu_credits"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("menu_options"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("menu_new"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("menu_play"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	setCharacterSize(50);
	setBubbleType(ABubble::Type::MainMenu);
	setCursorAtEnd();
}

void FakeMenu::onSelection(void)
{
	if (getIndexCursor() == 0u)
		m_quit = true;
}

void FakeMenu::setup(void)
{
	AMenuSelection::setup();
	setKeyboard(false);
}

void FakeMenu::update(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	if (m_quit == false)
	{
		AMenuSelection::update(frameTime, octoBubblePosition);
		Progress & progress = Progress::getInstance();
		progress.setBubbleNpc(false);
	}
}

void FakeMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (m_quit == false)
		AMenuSelection::draw(render, states);
}

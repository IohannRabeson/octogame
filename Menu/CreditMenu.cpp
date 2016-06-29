#include "CreditMenu.hpp"
#include "Progress.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CreditMenu::CreditMenu(void) :
	m_isSound(false)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	m_sound = audio.playSound(resources.getSound(OCTO_MONOLOGUE_OGG), 0.3f);
}

void CreditMenu::createMenus(void)
{
	addMenu(AMenu::getText("credits_programming") + L" :", nullptr);
	addMenu(L"Julien Balestrieri", nullptr);
	addMenu(L"François Corbel", nullptr);
	addMenu(L"Guillaume Lasset", nullptr);
	addMenu(L"Iohann Rabesson", nullptr);
	addMenu(L"", nullptr);
	addMenu(AMenu::getText("credits_graphics") + L" :", nullptr);
	addMenu(L"Lucien Coycault", nullptr);
	addMenu(L"", nullptr);
	addMenu(AMenu::getText("credits_sounds") + L" :", nullptr);
	addMenu(L"Pierre Glory", nullptr);
	addMenu(L"Jean-François Guerrin", nullptr);
	addMenu(L"Fabien You", nullptr);
	setIsFontSelect(false);
}

void CreditMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	if (getState() == AMenu::State::Active && m_isSound == false)
	{
		m_sound->play();
		m_isSound = true;
	}
	else if (getState() != AMenu::State::Active)
	{
		m_sound->pause();
		m_isSound = false;
	}
	AMenuSelection::update(frameTime, position);
}

#include "CreditMenu.hpp"
#include "Progress.hpp"

CreditMenu::CreditMenu(void)
{
}

void CreditMenu::createMenus(void)
{
	addMenu(L"Développement :", nullptr);
	addMenu(L"Julien Balestrieri", nullptr);
	addMenu(L"François Corbel", nullptr);
	addMenu(L"Guillaume Lasset", nullptr);
	addMenu(L"Iohann Rabesson", nullptr);
	addMenu(L"", nullptr);
	addMenu(L"Graphique :", nullptr);
	addMenu(L"Lucien Coycault", nullptr);
	addMenu(L"", nullptr);
	addMenu(L"Musique/Sons :", nullptr);
	addMenu(L"Pierre Glory", nullptr);
	addMenu(L"Jean-François Guerrin", nullptr);
	addMenu(L"Fabien You", nullptr);

}

void CreditMenu::update(sf::Time frameTime, sf::Vector2f const & position)
{
	AMenuSelection::update(frameTime, position);
}

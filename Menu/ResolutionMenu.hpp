#ifndef RESOLUTIONMENU_HPP
# define RESOLUTIONMENU_HPP

# include "AMenuSelection.hpp"
# include "EmptyMenu.hpp"

class ResolutionMenu : public AMenuSelection
{
public:
	ResolutionMenu(void);
	~ResolutionMenu(void) = default;

	void createMenus(void);
	void onSelection(void);
	void setupGraphics(sf::VideoMode mode);

private:
	std::vector<sf::VideoMode>	m_modes;
};

#endif

#ifndef BALLEMULTIPLIERMENU_HPP
# define BALLEMULTIPLIERMENU_HPP

# include "AMenuSelection.hpp"

class BalleMultiplierMenu : public AMenuSelection
{
public:
	BalleMultiplierMenu(void);
	~BalleMultiplierMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

#ifndef CHEATCODEMENU_HPP
# define CHEATCODEMENU_HPP

# include "AMenuSelection.hpp"

class CheatCodeMenu : public AMenuSelection
{
public:
	CheatCodeMenu(void);
	~CheatCodeMenu(void) = default;

	void createMenus();
};

#endif

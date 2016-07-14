#ifndef MENUTYPEMENU_HPP
# define MENUTYPEMENU_HPP

# include "AMenuSelection.hpp"

class MenuTypeMenu : public AMenuSelection
{
public:
	MenuTypeMenu(void);
	~MenuTypeMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

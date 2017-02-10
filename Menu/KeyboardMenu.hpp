#ifndef KEYBOARDMENU_HPP
# define KEYBOARDMENU_HPP

# include "AMenuSelection.hpp"

class KeyboardMenu : public AMenuSelection
{
public:
	KeyboardMenu(void);
	~KeyboardMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

#ifndef RESETMENU_HPP
# define RESETMENU_HPP

# include "AMenuSelection.hpp"

class ResetMenu : public AMenuSelection
{
public:
	ResetMenu(void);
	~ResetMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

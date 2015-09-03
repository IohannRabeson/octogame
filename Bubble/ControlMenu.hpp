#ifndef CONTROLMENU_HPP
# define CONTROLMENU_HPP

# include "AMenuSelection.hpp"

class ControlMenu : public AMenuSelection,
					public octo::DefaultKeyboardListener

{
public:
	ControlMenu(void);
	~ControlMenu(void) = default;

	void createMenus(void);
};

#endif

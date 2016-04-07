#ifndef OPTIONMENU_HPP
# define OPTIONMENU_HPP

# include "AMenuSelection.hpp"

class OptionMenu : public AMenuSelection
{
public:
	OptionMenu(void);
	~OptionMenu(void) = default;

	void createMenus();
};

#endif

#ifndef OPTIONMENU_HPP
# define OPTIONMENU_HPP

# include "AMenuSelection.hpp"
# include "VideoMenu.hpp"
# include "AudioMenu.hpp"

class OptionMenu : public AMenuSelection
{
public:
	OptionMenu(void);
	~OptionMenu(void) = default;

	void createMenus();
};

#endif

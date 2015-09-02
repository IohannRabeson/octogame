#ifndef OPTIONMENU_HPP
# define OPTIONMENU_HPP

# include "AMenuSelection.hpp"
# include "BubbleMenu.hpp"
# include "VideoMenu.hpp"
# include "AudioMenu.hpp"
# include "ControlMenu.hpp"

class OptionMenu : public AMenuSelection
{
public:
	OptionMenu(void);
	~OptionMenu(void) = default;

	void createMenus();
};

#endif

#ifndef SPIRITCHEATMENU_HPP
# define SPIRITCHEATMENU_HPP

# include "AMenuSelection.hpp"

class SpiritCheatMenu : public AMenuSelection
{
public:
	SpiritCheatMenu(void);
	~SpiritCheatMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

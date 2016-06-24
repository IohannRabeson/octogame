#ifndef RANDOMDISCOVERMENU_HPP
# define RANDOMDISCOVERMENU_HPP

# include "AMenuSelection.hpp"

class RandomDiscoverMenu : public AMenuSelection
{
public:
	RandomDiscoverMenu(void);
	~RandomDiscoverMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

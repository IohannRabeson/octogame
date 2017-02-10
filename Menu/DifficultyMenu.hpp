#ifndef DIFFICULTYMENU_HPP
# define DIFFICULTYMENU_HPP

# include "AMenuSelection.hpp"

class DifficultyMenu : public AMenuSelection
{
public:
	DifficultyMenu(void);
	~DifficultyMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

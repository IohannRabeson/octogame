#ifndef EMPTYMENU_HPP
# define EMPTYMENU_HPP

# include "AMenuSelection.hpp"

class EmptyMenu : public AMenuSelection
{
public:
	EmptyMenu(void);
	~EmptyMenu(void) = default;

	void createMenus();
};

#endif

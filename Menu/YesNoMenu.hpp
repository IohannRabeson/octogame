#ifndef YESNOMENU_HPP
# define YESNOMENU_HPP

# include "AMenuSelection.hpp"
# include "EmptyMenu.hpp"

class YesNoMenu : public AMenuSelection
{
public:
	YesNoMenu(void);
	~YesNoMenu(void) = default;

	void createMenus(void);
	void onSelection(void);

	// Will be virtual pure
	virtual void setIndex(void) = 0;
	virtual void actionYes(void) = 0;
	virtual void actionNo(void) = 0;
};

#endif

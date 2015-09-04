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
	void setIndex(std::size_t index);
	void action(void);
};

#endif

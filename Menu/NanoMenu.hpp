#ifndef NANOMENU_HPP
# define NANOMENU_HPP

# include "AMenuSelection.hpp"

class NanoMenu : public AMenuSelection
{
public:
	NanoMenu(void);
	~NanoMenu(void) = default;

	void createMenus();
	void onSelection();
};

#endif

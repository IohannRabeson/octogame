#ifndef VIDEOMENU_HPP
# define VIDEOMENU_HPP

# include "AMenuSelection.hpp"

class VideoMenu : public AMenuSelection
{
public:
	VideoMenu(void);
	~VideoMenu(void) = default;

	void createMenus(void);
};

#endif

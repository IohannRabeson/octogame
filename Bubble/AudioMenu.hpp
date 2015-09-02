#ifndef AUDIOMENU_HPP
# define AUDIOMENU_HPP

# include "AMenuSelection.hpp"
# include "BubbleMenu.hpp"

class AudioMenu : public AMenuSelection
{
public:
	AudioMenu(void);
	~AudioMenu(void) = default;

	void createMenus();
};

#endif

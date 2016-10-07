#ifndef PLAYENDMENU_HPP
# define PLAYENDMENU_HPP

# include "AMenuSelection.hpp"

class PlayEndMenu : public AMenuSelection
{
public:
	PlayEndMenu(void);
	~PlayEndMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::vector<std::wstring>	m_names;
};

#endif

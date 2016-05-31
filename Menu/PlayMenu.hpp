#ifndef PLAYMENU_HPP
# define PLAYMENU_HPP

# include "AMenuSelection.hpp"

class PlayMenu : public AMenuSelection
{
public:
	PlayMenu(void);
	~PlayMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::size_t					m_index;
	std::vector<std::wstring>	m_names;
};

#endif

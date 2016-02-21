#ifndef LEVELMENU_HPP
# define LEVELMENU_HPP

# include "AMenuSelection.hpp"

class LevelMenu : public AMenuSelection
{
public:
	LevelMenu(void);
	~LevelMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::size_t		m_index;
};

#endif

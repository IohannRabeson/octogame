#ifndef GLOBALVOLUMEMENU_HPP
# define GLOBALVOLUMEMENU_HPP

# include "AMenuSelection.hpp"

class GlobalVolumeMenu : public AMenuSelection
{
public:
	GlobalVolumeMenu(void);
	~GlobalVolumeMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::size_t		m_volume;
};

#endif

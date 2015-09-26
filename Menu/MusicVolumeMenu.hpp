#ifndef MUSICVOLUMEMENU_HPP
# define MUSICVOLUMEMENU_HPP

# include "AMenuSelection.hpp"

class MusicVolumeMenu : public AMenuSelection
{
public:
	MusicVolumeMenu(void);
	~MusicVolumeMenu(void) = default;

	void createMenus();
	void onSelection();
private:
	std::size_t m_volume;
};

#endif

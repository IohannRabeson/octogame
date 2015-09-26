#ifndef SOUNDVOLUMEMENU_HPP
# define SOUNDVOLUMEMENU_HPP

# include "AMenuSelection.hpp"

class SoundVolumeMenu : public AMenuSelection
{
public:
	SoundVolumeMenu(void);
	~SoundVolumeMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::size_t		m_volume;
};

#endif

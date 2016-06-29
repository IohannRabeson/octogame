#ifndef CREDITMENU_HPP
# define CREDITMENU_HPP

# include "AMenuSelection.hpp"
# include <AudioManager.hpp>

class CreditMenu : public AMenuSelection
{
public:
	CreditMenu(void);
	~CreditMenu(void) = default;

	void			createMenus(void);
	virtual void	update(sf::Time frameTime, sf::Vector2f const & position);

private:
	bool m_isSound;
	std::shared_ptr<sf::Sound> m_sound;
};

#endif

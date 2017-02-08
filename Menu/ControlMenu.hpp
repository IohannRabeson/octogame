#ifndef CONTROLMENU_HPP
# define CONTROLMENU_HPP

# include "AMenuSelection.hpp"

class ControlMenu : public AMenuSelection

{
public:
	ControlMenu(void);
	~ControlMenu(void) = default;

	void			createMenus(void);
	virtual void	update(sf::Time frameTime, sf::Vector2f const & position);

private:
	bool	m_isCreatedMoveMap;
	bool	m_isCreatedRepair;
	bool	m_isCreatedJump;
	bool	m_isCreatedDoubleJump;
	bool	m_isCreatedWaterJump;
	bool	m_isCreatedSlowFall;
	bool	m_isCreatedBalle;
};

#endif

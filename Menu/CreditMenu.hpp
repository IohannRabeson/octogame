#ifndef CREDITMENU_HPP
# define CREDITMENU_HPP

# include "AMenuSelection.hpp"

class CreditMenu : public AMenuSelection

{
public:
	CreditMenu(void);
	~CreditMenu(void) = default;

	void			createMenus(void);
	virtual void	update(sf::Time frameTime, sf::Vector2f const & position);
};

#endif

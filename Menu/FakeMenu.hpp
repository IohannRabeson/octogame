#ifndef FAKEMENU_HPP
# define FAKEMENU_HPP

# include "AMenuSelection.hpp"
# include <SFML/Graphics/RectangleShape.hpp>

class FakeMenu : public AMenuSelection
{
public:
	FakeMenu(void);
	~FakeMenu(void) = default;

	void	setup(void);
	void	update(sf::Time frameTime, sf::Vector2f const & position);
	void	draw(sf::RenderTarget & render, sf::RenderStates states) const;

	void	createMenus(void);
	void	onSelection(void);

private:
	bool	m_quit;
};

#endif

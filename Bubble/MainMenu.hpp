#ifndef MENU_HPP
# define MENU_HPP

# include "AMenuSelection.hpp"
# include "BubbleMenu.hpp"
# include "OptionMenu.hpp"
# include "ControlMenu.hpp"

# include <SFML/Graphics/RectangleShape.hpp>

class MainMenu : public AMenuSelection
{
public:
	MainMenu(void);
	~MainMenu(void) = default;

	void	setup(void);
	void	update(sf::Time frameTime, sf::Vector2f const & position);
	void	draw(sf::RenderTarget & render, sf::RenderStates states) const;

	void	createMenus(void);

private:
	sf::RectangleShape			m_filter;
};


#endif

#ifndef MENU_HPP
# define MENU_HPP

# include "AMenuSelection.hpp"
# include "BubbleText.hpp"
# include "OptionMenu.hpp"

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
	BubbleText					m_infoBubble;
	std::wstring				m_infoText;
	std::size_t					m_nanoCount;
	std::size_t					m_npcCount;
	std::size_t					m_npcMax;
};

#endif

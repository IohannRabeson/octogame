#ifndef MENU_HPP
# define MENU_HPP

# include "AMenuSelection.hpp"
# include "BubbleText.hpp"
# include "OptionMenu.hpp"
# include "NanoRobot.hpp"
# include "MenuNanoRobot.hpp"

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
	void	onSelection(void);

private:
	sf::RectangleShape							m_filter;
	bool										m_soundPlayed;
	std::wstring								m_progressionString;
	std::vector<std::unique_ptr<BubbleText>>	m_progressionBubble;
	std::vector<std::unique_ptr<NanoRobot>>		m_spirit;

	void updateNanoRobots(sf::Time frameTime, sf::Vector2f const & octoBubblePosition);
	void updateProgression(sf::Time frameTime);
};

#endif

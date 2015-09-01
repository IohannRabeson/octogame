#ifndef BUBBLEMENU_HPP
# define BUBBLEMENU_HPP

# include "ABubble.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Text.hpp>
# include <cstring>

class BubbleMenu : public ABubble
{
public:
	struct subMenu
	{
		sf::Text		text;
		sf::Vector2f	size;
		sf::Vector2f	position;
	};

	BubbleMenu(void);
	virtual ~BubbleMenu(void) = default;

	virtual void						setup(std::vector<std::string> const & subMenuTitle,
											  sf::Color const & color,
											  std::size_t characterSize = 0u);

	virtual sf::Vector2f				getContentSize(void) const;
	std::vector<sf::Vector2f> const &	getCursorPosition(void);
	virtual void						updateContent(sf::Time frameTime, sf::Vector2f const & position);
	virtual void						drawContent(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	static constexpr std::size_t	m_characterSize = 20u;

	sf::Vector2f					m_contentSize;
	sf::Font						m_font;
	//TODO: For mouse use return this array
	std::vector<subMenu>			m_subMenus;
	std::vector<sf::Vector2f>		m_cursorPosition;
};

#endif

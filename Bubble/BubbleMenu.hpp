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
	BubbleMenu(void);
	virtual ~BubbleMenu(void) = default;

	virtual void			setup(std::string const & phrase,
									sf::Color const & color,
									std::size_t characterSize = 0u);

	virtual sf::Vector2f	getContentSize(void) const;
	virtual void			updateContent(sf::Time frameTime, sf::Vector2f const & position);
	virtual void			drawContent(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	void setupBlocString(void);

	static constexpr float			m_bubbleWidth = 500.f;
	static constexpr std::size_t	m_characterSize = 20u;

	sf::Vector2f					m_contentSize;
	sf::Font						m_font;
	sf::Text						m_text;
	std::size_t						m_characterPerLine;
	std::size_t						m_lineCount;
	std::string						m_phrase;
};

#endif

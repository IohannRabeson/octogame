#ifndef BUBBLETEXT_HPP
# define BUBBLETEXT_HPP

# include "ABubble.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Text.hpp>
# include <cstring>

class BubbleText : public ABubble
{
public:
	BubbleText(void);
	virtual ~BubbleText(void) = default;

	virtual void					setup(std::wstring const & phrase,
											sf::Color const & color,
											std::size_t characterSize = 0u);

	virtual sf::Vector2f const &	getContentSize(void) const;
	virtual void					updateContent(sf::Time frameTime, sf::Vector2f const & position);
	virtual void					drawContent(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	void setupBlocString(void);

	static constexpr float			m_bubbleWidth = 350.f;
	static constexpr std::size_t	m_characterSize = 20u;

	sf::Vector2f					m_contentSize;
	sf::Font						m_font;
	sf::Text						m_text;
	std::size_t						m_characterPerLineMax;
	std::size_t						m_characterPerLine;
	std::size_t						m_lineCount;
	std::wstring					m_phrase;
};

#endif

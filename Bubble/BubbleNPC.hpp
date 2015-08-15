#ifndef BUBBLENPC_HPP
# define BUBBLENPC_HPP

# include "ABubble.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Text.hpp>
# include <cstring>

class BubbleNPC : public ABubble
{
public:
	BubbleNPC(void);
	virtual ~BubbleNPC(void) = default;

	virtual void		setup(std::string const & phrase,
								sf::Color const & color,
								std::size_t characterSize = 0u);

	virtual void		update(sf::Time frameTime,
								octo::VertexBuilder& builder);

	virtual sf::Text	getText(void) const;
	virtual void		createExtension(sf::Vector2f const & position,
										octo::VertexBuilder & builder);

private:
	static constexpr float			m_bubbleWidth = 500.f;
	static constexpr float			m_sizeCorner = 50.f;
	static constexpr std::size_t	m_characterSize = 30u;

	std::string						m_phrase;
	sf::Color						m_color;
	sf::Vector2f					m_size;
	sf::Font						m_font;
	sf::Text						m_text;
	std::size_t						m_characterPerLine;
	std::size_t						m_lineCount;

	DecorAnimator					m_animator;
	float							m_animation;

	void setupBlocString(void);
};

#endif

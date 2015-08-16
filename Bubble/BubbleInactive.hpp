#ifndef BUBBLEINACTIVE_HPP
# define BUBBLEINACTIVE_HPP

# include "ABubble.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Text.hpp>
# include <cstring>

class BubbleInactive : public ABubble
{
public:
	BubbleInactive(void);
	virtual ~BubbleInactive(void) = default;

	virtual void		setup(std::string const & phrase,
								sf::Color const & color);

	virtual void		update(sf::Time frameTime,
								octo::VertexBuilder& builder);

	virtual sf::Text	getText(void) const;
	virtual void		createExtension(sf::Vector2f const & position,
										octo::VertexBuilder & builder);

private:
	static constexpr float			m_sizeCorner = 25.f;

	sf::Vector2f					m_size;
	sf::Text						m_text;


	void createQuotePart(sf::Vector2f const & size,
						sf::Vector2f const & origin,
						sf::Color const & color,
						octo::VertexBuilder & builder);
};

#endif

#ifndef ABUBBLE_HPP
# define ABUBBLE_HPP

# include "DecorAnimator.hpp"
# include <SFML/Graphics/Text.hpp>
# include <VertexBuilder.hpp>

class ABubble
{
public:
	ABubble(void);
	virtual ~ABubble(void) = default;

	virtual void		update(sf::Time frameTime,
								octo::VertexBuilder& builder) = 0;

	virtual sf::Text	getText(void) const = 0;
	virtual void		createExtension(sf::Vector2f const & position,
										octo::VertexBuilder & builder) = 0;


	sf::Vector2f		getTextUpLeft(void) const;
	sf::Vector2f		getPosition(void) const;
	sf::Color			getColor(void) const;
	bool				isActive(void) const;
	void				setPosition(sf::Vector2f const & position);
	void				setColor(sf::Color const & color);
	void				setActive(bool isActive);

	void				createOctogon(sf::Vector2f const & size,
										float sizeCorner,
										sf::Vector2f const & origin,
										sf::Color const & color,
										octo::VertexBuilder& builder);

private:
	sf::Vector2f	m_textUpLeft;
	sf::Vector2f	m_position;
	sf::Color		m_color;
	bool			m_isActive;
};

#endif

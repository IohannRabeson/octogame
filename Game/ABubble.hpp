#ifndef ABUBBLE_HPP
# define ABUBBLE_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Text.hpp>

class ABubble
{
public:
	ABubble(void);
	virtual ~ABubble(void) = default;

	virtual void setup(void);
	virtual sf::Text getText(void) const;
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder);

private:
	sf::Vector2f			m_size;
	sf::Vector2f			m_sizeCorner;
	sf::Color				m_color;
	sf::Text				m_text;
	sf::Vector2f			m_textUpLeft;
	sf::Vector2f			m_extensionPos;

	DecorAnimator			m_animator;
	float					m_animation;

	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						sf::Color const & color,
						octo::VertexBuilder& builder);

	void createABubble(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					sf::Color color,
					octo::VertexBuilder& builder);
};

#endif

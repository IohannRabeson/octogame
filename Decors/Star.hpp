#ifndef STAR_HPP
# define STAR_HPP

# include "ShineBuilder.hpp"
# include "DecorAnimator.hpp"

class GameClock;

class Star : public ShineBuilder
{
public:
	Star(void);
	Star(GameClock * clock);
	virtual ~Star(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_sizeHeart;
	DecorAnimator	m_animator;
	float			m_animation;

	sf::Vector2f	m_glowSize;
	sf::Vector2f	m_glowSizeCorner;

	GameClock		*m_clock;
};

#endif

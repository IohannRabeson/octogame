#ifndef STAR_HPP
# define STAR_HPP

# include "AShineBuilder.hpp"
# include "DecorAnimator.hpp"

class SkyCycle;

class Star : public AShineBuilder
{
public:
	Star(void);
	Star(SkyCycle * cycle);
	virtual ~Star(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool dieOutOfScreen(void);

private:
	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_sizeHeart;
	DecorAnimator	m_animator;
	float			m_animation;
	int				m_detailPriority;

	sf::Vector2f	m_glowSize;
	sf::Vector2f	m_glowSizeCorner;

	SkyCycle		*m_cycle;
};

#endif

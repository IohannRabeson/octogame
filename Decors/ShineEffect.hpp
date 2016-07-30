#ifndef SHINEEFFECT_HPP
# define SHINEEFFECT_HPP

# include "AShineBuilder.hpp"
# include "DecorAnimator.hpp"

class ShineEffect : public AShineBuilder
{
public:
	ShineEffect(void);
	virtual ~ShineEffect(void) = default;

	void		 setSize(sf::Vector2f const & size);
	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);
	DecorAnimator const & getAnimator(void);

private:
	sf::Vector2f	m_position;
	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_sizeHeart;
	float			m_angle;
	DecorAnimator	m_animator;
	float			m_animation;
	bool			m_isShineEffect;
	bool			m_isSound;

	sf::Vector2f	m_glowSize;
	sf::Vector2f	m_glowSizeCorner;

	void playSound(ABiome & biome, sf::Vector2f const & position);
};

#endif

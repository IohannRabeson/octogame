#ifndef STAR_HPP
# define STAR_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <VertexBuilder.hpp>
# include <vector>

class Star : public ADecor
{
public:
	Star(void);
	virtual ~Star(void) = default;

	void createStar(sf::Vector2f const & size,
					sf::Vector2f const & sizeHeart,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);

	void createGlow(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_sizeHeart;
	sf::Time		m_liveTime;
	DecorAnimator	m_animator;
	float			m_animation;

	sf::Vector2f	m_glowSize;
	sf::Vector2f	m_glowSizeCorner;
};

#endif

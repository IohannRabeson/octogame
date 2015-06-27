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
	Star(sf::Vector2f const & size, sf::Color const & color, float const angle);
	virtual ~Star(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_sizeHeart;
	float			m_angle;
	DecorAnimator	m_animator;
	float			m_animation;

	sf::Vector2f	m_glowSize;
	sf::Vector2f	m_glowSizeCorner;

	void createStar(sf::Vector2f const & size,
					sf::Vector2f const & sizeHeart,
					sf::Vector2f const & origin,
					sf::Color const & color,
					float const cosAngle,
					float const sinAngle,
					octo::VertexBuilder& builder);

	void createGlow(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					sf::Color const & color,
					float const cosAngle,
					float const sinAngle,
					octo::VertexBuilder& builder);

private:
	static void rotateVec(sf::Vector2f & vector,
							float const cosAngle,
							float const sinAngle);
};

#endif

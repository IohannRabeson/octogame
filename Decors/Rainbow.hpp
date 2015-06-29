#ifndef RAINBOW_HPP
# define RAINBOW_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class Rainbow : public ADecor
{
public:
	Rainbow(void);
	virtual ~Rainbow(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	std::vector<std::vector<sf::Vector2f>>	m_rainbow;
	sf::Vector2f							m_size;
	std::size_t								m_partCount;
	std::size_t								m_stripeCount;
	float									m_cos;
	float									m_sin;

	DecorAnimator							m_animator;
	float									m_animation;

	void createRainbowPart(sf::Vector2f const & size,
							sf::Vector2f const & origin,
							std::vector<sf::Vector2f> & start,
							std::vector<sf::Vector2f> & end,
							std::size_t stripeCount,
							octo::VertexBuilder& builder);

/*
	void createRainbow(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					std::size_t partCount,
					sf::Color color,
					octo::VertexBuilder& builder);
*/
private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
};

#endif

#ifndef ASHINEBUILDER_HPP
# define ASHINEBUILDER_HPP

# include "ADecor.hpp"
# include <VertexBuilder.hpp>

class AShineBuilder : public ADecor
{
public:
	AShineBuilder(void);
	virtual ~AShineBuilder(void) = default;

	virtual void setup(ABiome& biome) = 0;
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome) = 0;

	void createStar(sf::Vector2f const & size,
					sf::Vector2f const & sizeHeart,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder,
					float const angle = 0.f,
					float const cosAngle = 0.f,
					float const sinAngle = 0.f);

	void createGlow(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder,
					float const angle = 0.f,
					float const cosAngle = 0.f,
					float const sinAngle = 0.f);

private:
	static void rotateVec(sf::Vector2f & vector,
							float const cosAngle,
							float const sinAngle);
};

#endif

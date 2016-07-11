#ifndef GRASS_HPP
# define GRASS_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class Grass : public ADecor
{
public:
	Grass(bool onInstance = false);
	virtual ~Grass(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Color				m_color;
	DecorAnimator			m_animator;
	float					m_animation;
	float					m_animationSpeed;

	void createGrass(sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif

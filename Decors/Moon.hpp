#ifndef MOON_HPP
# define MOON_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class Moon : public ADecor
{
public:
	Moon(void);
	virtual ~Moon(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						sf::Color const & color,
						octo::VertexBuilder& builder);

	void createMoon(sf::Vector2f const & size,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);

	DecorAnimator			m_animator;
	float					m_animation;

	sf::Vector2f			m_size;
	sf::Color				m_color;
};

#endif

#ifndef SKY_HPP
# define SKY_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class SkyCycle;

class Sky : public ADecor
{
public:
	Sky(void);
	Sky(SkyCycle * cycle);
	virtual ~Sky(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	void createSky(sf::Vector2f const & size,
					sf::Vector2f const & origin,
					sf::Color const & colorUp,
					sf::Color const & colorDown,
					octo::VertexBuilder& builder);

	sf::Color	m_colorUp;
	sf::Color	m_colorDown;
	SkyCycle	*m_cycle;
};

#endif

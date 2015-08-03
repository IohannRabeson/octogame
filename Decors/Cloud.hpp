#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include "RainSystem.hpp"
# include "Lightning.hpp"
# include <SFML/Graphics/Color.hpp>

class SkyCycle;

class Cloud : public ADecor
{
struct OctogonValue
{
	sf::Vector2f	size;
	sf::Vector2f	sizeCorner;
	sf::Vector2f	origin;
};

public:
	Cloud(void);
	Cloud(SkyCycle * cycle);
	virtual ~Cloud(void);

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool isDisabledIfOutOfScreen()const;
private:
	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						sf::Color color,
						octo::VertexBuilder& builder);

	void createCloud(std::vector<OctogonValue> const & values,
					sf::Vector2f const & origin,
					std::size_t partCount,
					sf::Color const & color,
					octo::VertexBuilder& builder);

	void setupLightning(void);

	void newCloud(ABiome & biome);

	sf::Vector2f				m_size;
	std::size_t					m_partCount;
	sf::Color					m_color;
	std::vector<OctogonValue>	m_values;

	DecorAnimator				m_animator;
	float						m_animation;

	std::vector<RainSystem *>	m_rain;
	std::vector<sf::Vector2f>	m_rainUpLeft;

	SkyCycle *					m_cycle;

	Lightning					m_lightning;
	sf::Vector2f				m_p0;
	sf::Vector2f				m_p1;
};

#endif

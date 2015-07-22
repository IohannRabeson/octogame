#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include "RainSystem.hpp"
# include <SFML/Graphics/Color.hpp>

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
	virtual ~Cloud(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f				m_size;
	std::size_t					m_partCount;
	sf::Color					m_color;
	std::vector<OctogonValue>	m_values;

	DecorAnimator				m_animator;
	float						m_animation;

	RainSystem					m_rain;

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

	void newCloud(ABiome & biome);
};

#endif

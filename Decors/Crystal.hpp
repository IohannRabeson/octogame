#ifndef CRYSTAL_HPP
# define CRYSTAL_HPP

# include "ADecor.hpp"
# include "ShineEffect.hpp"
# include "DecorAnimator.hpp"
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Color.hpp>

class Crystal : public ADecor
{
public:
	Crystal(void);
	virtual ~Crystal(void) = default;

	virtual void	setup(ABiome& biome);
	virtual void	update(sf::Time frameTime,
							octo::VertexBuilder& builder,
							ABiome& biome);

private:
	struct CrystalValue
	{
		sf::Vector2f			size;
		sf::Color				color;
		float					angle;
	};

	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::size_t					m_partCount;
	std::vector<CrystalValue>	m_values;
	DecorAnimator				m_animator;
	float						m_animation;

	ShineEffect					m_shine;
	std::vector<sf::Vector2f>	m_up;
	std::vector<sf::Vector2f>	m_upLeft;
	std::size_t					m_shineCrystalNumber;
	sf::Time					m_shineTimer;
	sf::Time					m_shineTimerMax;

	void createPolygon(sf::Vector2f const & size,
								sf::Vector2f const & origin,
								float const angle,
								sf::Color color,
								sf::Vector2f & up,
								sf::Vector2f & upLeft,
								octo::VertexBuilder & builder);

	void createCrystal(std::vector<CrystalValue> const & values,
						sf::Vector2f const & origin,
						octo::VertexBuilder & builder);
};

#endif

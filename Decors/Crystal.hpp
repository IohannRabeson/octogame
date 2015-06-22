#ifndef CRYSTAL_HPP
# define CRYSTAL_HPP

# include "ADecor.hpp"
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
	unsigned int				m_partCount;
	std::vector<CrystalValue>	m_values;
	float						m_animation;

	std::vector<sf::Vector2f>	m_up;
	//TODO: Implement shine effet
	/*
	unsigned int				m_shineCrystalNumber;
	unsigned int				m_shineVertexNumber;
	sf::Time					m_shineTimer;
	sf::Time					m_shineTimerMax;
	*/

	sf::Vector2f createPolygon(sf::Vector2f const & size,
								sf::Vector2f const & origin,
								float const angle,
								sf::Color color,
								octo::VertexBuilder & builder);

	void createCrystal(std::vector<CrystalValue> const & values,
						sf::Vector2f const & origin,
						octo::VertexBuilder & builder);

	static void rotateVec(sf::Vector2f & vector,
							float const cosAngle,
							float const sinAngle);
	//TODO: To delete
	float randomFloat(float min, float max);


};

#endif

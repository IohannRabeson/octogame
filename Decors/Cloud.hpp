#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "ADecor.hpp"
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Color.hpp>

class Cloud : public ADecor
{
struct OctogonValue
{
	sf::Vector2f			size;
	sf::Vector2f			origin;
	float					sizeUp;
	float					sizeDown;
	float					sizeRec;
};

public:
	Cloud(void);
	virtual ~Cloud(void) = default;

	virtual void	setup(ABiome& biome);
	virtual void	update(sf::Time frameTime,
						   octo::VertexBuilder& builder,
						   ABiome& biome);

	float randomFloat(float min, float max);

private:
	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::size_t					m_partCount;
	std::vector<OctogonValue>	m_values;
	float						m_animation;
	sf::Time					m_lifeTime;

	//TODO: Change this
	int							m_alpha;

	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & origin,
						sf::Color color,
						float const sizeUp,
						float const sizeDown,
						float const sizeRec,
						octo::VertexBuilder& builder);

	void createCloud(std::vector<OctogonValue> const & values,
					sf::Vector2f const & originCloud,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif

#ifndef GROUNDGRASS_HPP
# define GROUNDGRASS_HPP

# include "ADecor.hpp"
# include <SFML/Graphics/Color.hpp>

class GroundGrass : public ADecor
{
public:
	GroundGrass(bool onInstance = false);
	virtual ~GroundGrass(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Color				m_color;
	std::size_t				m_type;
	float					m_deep;
	bool					m_onInstance;

	void createGroundGrass(std::size_t type,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif

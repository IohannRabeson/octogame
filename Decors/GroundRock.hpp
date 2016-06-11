#ifndef GROUNDROCK_HPP
# define GROUNDROCK_HPP

# include "ADecor.hpp"
# include <SFML/Graphics/Color.hpp>

class GroundRock : public ADecor
{
public:
	GroundRock(bool onInstance = false);
	virtual ~GroundRock(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Color				m_color;
	std::size_t				m_type;
	float					m_deep;
	bool					m_onInstance;

	void createGroundRock(std::size_t type,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif

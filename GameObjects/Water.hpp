#ifndef WATER_HPP
# define WATER_HPP

# include <SFML/Graphics.hpp>
# include "AGameObject.hpp"
# include "IPlaceable.hpp"

class ABiome;

class Water : public AGameObject<GameObjectType::Water>, public sf::Drawable
{
public:
	Water(ABiome & biome);
	virtual ~Water(void);

	sf::Vector2f const & getPosition(void) const;
	void setPosition(sf::Vector2f const & position);
	void addMapOffset(float width);

	void update(sf::Time frametime);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	sf::Texture				m_distorsionTexture;
	sf::RectangleShape		m_rectLeft;
	sf::RectangleShape		m_rectRight;
	sf::Shader				m_shader;
	sf::Time				m_waveCycle;
	float					m_width;
	float					m_limit;
	std::size_t				m_shaderIndex;

};

#endif

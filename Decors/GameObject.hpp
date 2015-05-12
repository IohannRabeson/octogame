#ifndef GAMEOBJECT_HPP
# define GAMEOBJECT_HPP

# include <SFML/Graphics.hpp>

// That class is only used for the position
class GameObject
{
public:
	GameObject(void);
	GameObject(float x, float y, sf::Vertex * p_position);
	virtual ~GameObject(void);

	void setPosition(sf::Vector2f const & p_pos);
	sf::Vector2f const & getPosition(void) const;
	// Called by the StaticObjectManager after each transition
	void updatePosition(sf::Vertex * p_pos);

protected:
	sf::Vector2f		m_origin;
	sf::Vector2f		m_initialPosition;
	// Pointer to the upLeftVertex of the tile at m_initialPosition
	// There is also three other vertices at m_position[1, 2, 3]
	sf::Vertex *		m_vertexPosition;

};

#endif

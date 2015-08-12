#ifndef PORTAL_HPP
# define PORTAL_HPP

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include "AGameObject.hpp"

class Portal : public AGameObject<GameObjectType::Portal>
{
public:
	Portal(void);
	virtual ~Portal(void) = default;

	inline void setPosition(sf::Vector2f const & position) { m_position = position; }
	inline sf::Vector2f const & getPosition(void) const { return m_position; }

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render)const;

private:
	sf::Vector2f		m_position;
	sf::Shader			m_shader;
	float				m_radius;

};

#endif

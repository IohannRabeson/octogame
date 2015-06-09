#ifndef CIRCLE_HPP
# define CIRCLE_HPP

# include "AShape.hpp"

class Circle : public AShape
{
public:
	Circle(void);
	virtual ~Circle(void) = default;

	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius(void) const { return m_radius; }

	inline sf::Vector2f const & getCenter(void);

	virtual sf::Rect<float> const & getGlobalBounds(void);
	virtual void rotate(float angle);
	virtual void debugDraw(sf::RenderTarget & render);

private:
	sf::Vector2f		m_rotatedCenter;
	float			m_radius;

};

#endif

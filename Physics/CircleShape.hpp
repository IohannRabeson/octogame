#ifndef CIRCLESHAPE_HPP
# define CIRCLESHAPE_HPP

# include "AShape.hpp"

class CircleShape : public AShape
{
public:
	CircleShape(void);
	virtual ~CircleShape(void) = default;

	inline void setRadius(float radius) { m_radius = radius; }
	inline float getRadius(void) const { return m_radius; }

	virtual sf::Vector2f const & getCenter(void);

	virtual sf::Rect<float> const & getGlobalBounds(void);
	virtual void rotate(float angle);
	virtual void debugDraw(sf::RenderTarget & render);

private:
	sf::Vector2f		m_rotatedCenter;
	sf::Vector2f		m_center;
	sf::Rect<float>		m_globalBounds;
	float			m_radius;

};

#endif

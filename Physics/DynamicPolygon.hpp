#ifndef DYNAMICPOLYGON_HPP
# define DYNAMICPOLYGON_HPP

# include "Polygon.hpp"

class DynamicPolygon : public Polygon
{
public:
	DynamicPolygon(std::size_t p_verticesCount);
	virtual ~DynamicPolygon(void);

	void rotate(float p_angle);
	void move(sf::Vector2f const & p_translation);
	sf::Rect<float> const & getGlobalBounds(void);

	inline void addVelocity(sf::Vector2f const & v) { m_velocity += v; }
	inline void addVelocity(float x, float y) { m_velocity.x += x; m_velocity.y += y; }
	inline sf::Vector2f const & getVelocity(void) const { return m_velocity; }

	// Compute the desire transformation
	virtual void update(float pf_deltatime) = 0;
	// Apply transformation computed by the CollisionManager
	virtual void applyTransform(void);


protected:
	sf::Rect<float>		m_globalBounds;
	sf::Vector2f		m_velocity;

private:
	DynamicPolygon(void) = delete;

};

#endif

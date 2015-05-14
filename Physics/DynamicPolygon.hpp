#ifndef DYNAMICPOLYGON_HPP
# define DYNAMICPOLYGON_HPP

# include "Polygon.hpp"

class DynamicPolygon : public Polygon
{
public:
	virtual ~DynamicPolygon(void);

	void rotate(float p_angle);
	void move(sf::Vector2f const & p_translation);
	sf::Rect<float> const & getGlobalBounds(void);

	inline void addVelocity(sf::Vector2f const & v) { m_velocity += v; }
	inline void addVelocity(float x, float y) { m_velocity.x += x; m_velocity.y += y; }
	inline sf::Vector2f const & getVelocity(void) const { return m_velocity; }
	inline bool applyGravity(void) const { return m_applyGravity; }
	inline int getCollideMask(void) const { return m_collideMask; }

	// Compute the desire transformation
	virtual void update(float pf_deltatime) = 0;
	// Apply transformation computed by the CollisionManager
	virtual void applyTransform(void);
	// Called when a collision occurs
	virtual void onCollision(Polygon * p_polygon) = 0;

protected:
	DynamicPolygon(std::size_t p_verticesCount, CollideType p_collideType, bool p_applyGravity);

	sf::Rect<float>		m_globalBounds;
	sf::Vector2f		m_velocity;
	bool			m_applyGravity;
	int			m_collideMask;

private:
	DynamicPolygon(void) = delete;

};

#endif

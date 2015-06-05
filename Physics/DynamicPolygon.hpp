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
	inline sf::Vector2f const & getOldVelocity(void) const { return m_oldVelocity; }
	inline bool applyGravity(void) const { return m_applyGravity; }
	inline int getCollideMask(void) const { return m_collideMask; }
	inline void setPosition(sf::Vector2f const & position) { move(position - getCenter()); }

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
	//TODO: uint64
	std::size_t		m_collideMask;
	sf::Vector2f	m_oldVelocity;

private:
	DynamicPolygon(void) = delete;

};

#endif

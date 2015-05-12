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
	virtual void updateVelocity(float pf_deltatime) = 0;
	virtual void update(float pf_deltatime);

	// TODO: remove
	sf::Vector2f		m_velocity;

protected:
	sf::Rect<float>		m_globalBounds;

};

#endif

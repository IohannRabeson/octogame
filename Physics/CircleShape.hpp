#ifndef CIRCLESHAPE_HPP
# define CIRCLESHAPE_HPP

# include "AShape.hpp"

class CircleShape : public AShape
{
public:
	CircleShape(void);
	virtual ~CircleShape(void) = default;

	/*! Set the position of the center
	 *
	 * \param centerPosition The new center position
	 */
	inline void setBaryCenterPosition(sf::Vector2f const & centerPosition) { setBaryCenterPosition(centerPosition.x, centerPosition.y); }

	/*! Set the position of the center
	 * You must define the radius first
	 *
	 * \param centerPosition The new center position
	 */
	inline void setBaryCenterPosition(float x, float y) { setPosition(x - m_radius, y - m_radius); }

	/*! Set the radius
	 *
	 * \param radius The new radius
	 */
	void setRadius(float radius);

	/*! Get the radius */
	inline float getRadius(void) const { return m_radius; }

	/*! Get the global bounds
	 * The AABB is recomputed at each rotation
	 *
	 * \see rotate
	 */
	inline virtual sf::Rect<float> const & getGlobalBounds(void) const { return m_globalBounds; }

	/*! Get the center of the object
	 * The center is recomputed at each rotation
	 *
	 * \see rotate
	 */
	inline virtual sf::Vector2f const & getBaryCenter(void) const { return m_baryCenter; }

	/*! Use to draw debug information */
	virtual void debugDraw(sf::RenderTarget & render);

private:
	sf::Vector2f		m_baryCenter;
	sf::Vector2f		m_initialBaryCenter;
	sf::Rect<float>		m_globalBounds;
	float				m_radius;

	/*! Compute the shape once per frame */
	virtual void computeShape(void);

};

#endif

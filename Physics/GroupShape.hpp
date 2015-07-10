#ifndef GROUPSHAPE_HPP
# define GROUPSHAPE_HPP

# include <vector>
# include "RectangleShape.hpp"

class CircleShape;
class PolygonShape;

/*!
 * \ingroup Physic
 * \class GroupShape
 * Class representing a group of shapes
 * This class manage the memory of the shapes.
 * The RectangleShape inherited represent the physic attribute of the group,
 * it's the only one which compute the new position of the GroupShape.
 */
class GroupShape : public RectangleShape
{
public:
	GroupShape(void);
	virtual ~GroupShape(void);

	/*! Add RectangleShape
	 * The shape is register in the PhysicsEngine with a special status,
	 * it won't be updated by the PhysicsEngine, but by the GroupShape.
	 * The PhysicsEngine only detect collisions between the trigger shape and other shapes, it won't resolve it.
	 *
	 * \return The new rectangle shape
	 * \see PhysicsEngine, RectangleShape
	 */
	RectangleShape * addRectangleShape(void);

	/*! Add CircleShape
	 * The shape is register in the PhysicsEngine with a special status,
	 * it won't be updated by the PhysicsEngine, but by the GroupShape.
	 * The PhysicsEngine only detect collisions between the trigger shape and other shapes, it won't resolve it.
	 *
	 * \return The new rectangle shape
	 * \see PhysicsEngine, CircleShape
	 */
	CircleShape * addCircleShape(void);

	inline RectangleShape const & getPhysicShape(void) const { return *this; }
	inline std::vector<PolygonShape *> const & getPolygons(void) const { return m_polyShapes; }
	inline std::vector<CircleShape *> const & getCircles(void) const { return m_circleShapes; }

	/*! Get the global bounds
	 * The AABB is recomputed at each rotation
	 *
	 * \see rotate
	 */
	inline sf::FloatRect const & getGroupGlobalBounds(void) const { return m_groupGlobalBounds; }

	/*! Use to draw debug information
	 */
	virtual void debugDraw(sf::RenderTarget & render);

protected:
	/*! Compute the shape once per frame */
	virtual void computeShape(void);

private:
	std::vector<AShape *>		m_shapes;
	std::vector<PolygonShape *>	m_polyShapes;
	std::vector<CircleShape *>	m_circleShapes;
	sf::FloatRect				m_groupGlobalBounds;

};

#endif

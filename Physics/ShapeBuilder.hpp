#ifndef SHAPEBUILDER_HPP
# define SHAPEBUILDER_HPP

# include <cstddef>

class ConvexShape;
class RectangleShape;
class GroupShape;
class CircleShape;
class TileShape;

/*!
 * \ingroup Physic
 * \class IShapeBuilder
 * Interface that allow to create new Shapes and register them in the PhysicsEngine
 * PhysicsEngine manage the memory of the allocated shapes
 */
class ShapeBuilder
{
public:
	virtual ~ShapeBuilder(void) = default;

	/*! Create e new ConvexShape
	 *
	 * \param inGroup Whether inGroup is true, the shape will not be updated by the
	 * physics engine but by the group it belong's to
	 */
	ConvexShape * createConvex(bool inGroup = false);

	/*! Create a new Circleshape
	 *
	 * \param inGroup Whether inGroup is true, the shape will not be updated by the
	 * physics engine but by the group it belong's to
	 */
	CircleShape * createCircle(bool inGroup = false);

	/*! Create a new RectangleShape
	 *
	 * \param inGroup Whether inGroup is true, the shape will not be updated by the
	 * physics engine but by the group it belong's to
	 */
	RectangleShape * createRectangle(bool inGroup = false);

	/*! Create a new GroupShape */
	GroupShape * createGroupShape(void);

	/*! Create a new TileShape */
	TileShape * createTile(std::size_t x, std::size_t y);

};

#endif

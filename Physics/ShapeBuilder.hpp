#ifndef SHAPEBUILDER_HPP
# define SHAPEBUILDER_HPP

# include <cstddef>

class ConvexShape;
class RectangleShape;
class CircleShape;

/*!
 * \ingroup Physic
 * \class IShapeBuilder
 * Interface that allow to create new Shapes and register them in the PhysicsEngine
 */
class ShapeBuilder
{
public:
	virtual ~ShapeBuilder(void) = default;

	/*! Create e new ConvexShapê */
	ConvexShape * createConvex(void);

	/*! Create a new Circleshape */
	CircleShape * createCircle(void);

	/*! Create a new RectangleShape */
	RectangleShape * createRectangle(void);

	/*! Create a new ConvexShape */
	ConvexShape * createTile(std::size_t x, std::size_t y);

};

#endif

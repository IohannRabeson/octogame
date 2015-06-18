#ifndef ISHAPEBUILDER_HPP
# define ISHAPEBUILDER_HPP

class ConvexShape;
class RectangleShape;
class CircleShape;

/*!
 * \ingroup Physic
 * \class IShapeBuilder
 * Interface that allow to create new Shapes and register them in the PhysicsEngine
 */
class IShapeBuilder
{
public:
	virtual ~IShapeBuilder(void) = default;

	/*! Create e new ConvexShapê */
	virtual ConvexShape * createConvex(void) = 0;

	/*! Create a new Circleshape */
	virtual CircleShape * createCircle(void) = 0;

	/*! Create a new RectangleShape */
	virtual RectangleShape * createRectangle(void) = 0;

	/*! Create a new RectangleShape */
	virtual ConvexShape * createTile(std::size_t x, std::size_t y) = 0;

};

#endif

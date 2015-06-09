#ifndef ICONTACTLISTENER_HPP
# define ICONTACTLISTENER_HPP

class Shape;

/*!
 * \ingroup Physic
 * \class IContactListener
 * Interface that must be implemented to manage collision behavior
 */
class IContactListener
{
public:
	virtual ~IContactListener(void) = default;

	virtual void onShapeCollision(Shape * shapeA, Shape * shapeB) = 0;
	//TODO: virtual void onRayCollision(Shape * shapeA, Shape * shapeB) = 0;

};

#endif

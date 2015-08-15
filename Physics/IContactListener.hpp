#ifndef ICONTACTLISTENER_HPP
# define ICONTACTLISTENER_HPP

class AShape;

/*!
 * \ingroup Physic
 * \class IContactListener
 * Interface that must be implemented to manage collision behavior
 */
class IContactListener
{
public:
	virtual ~IContactListener(void) = default;

	virtual void onShapeCollision(AShape * shapeA, AShape * shapeB) = 0;
	virtual void onTileShapeCollision(TileShape * tileShape, AShape * shape) = 0;
	//TODO: virtual void onRayCollision() = 0;

};

#endif

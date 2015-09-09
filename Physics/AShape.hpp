#ifndef SHAPE_HPP
# define SHAPE_HPP

# include <SFML/Graphics.hpp>
# include "AGameObject.hpp"

/*!
 * \ingroup Physic
 * \class AShape
 * Abstract class representing a physic shape
 *
 * \code
 * Character * octo = new Character(); // Character inherits from AGameObject
 * ShapeBuilder builder = PhysicsEngine::getInstance();
 * AShape * box = builder.createRectangle();
 * box->setPosition(10, 15);
 * box->setGameObject(octo);
 * octo->setBox(box);
 * \endcode
 *
 */
class AShape
{
public:
	/*! Enum representing the type of collision */
	enum class Type : std::uint8_t
	{
		e_static = 0u,		///< Don't move, affect other objects
		e_dynamic = 1u,		///< Move, affect other objects
		e_kinematic = 2u,	///< Move, don't get affected by other objects
		e_trigger = 3u		///< Don't move, don't affect other objects
	};

	AShape(void);
	virtual ~AShape(void) = default;

	/*! Set the velocity
	 * The velocity will be used to resolve the collision in the Engine
	 *
	 * \param velocity New velocity
	 * \see move, PhysicsEngine
	 */
	inline void setVelocity(sf::Vector2f const & velocity) { setVelocity(velocity.x, velocity.y); }

	/*! Set the velocity
	 * The velocity will be used to resolve the collision in the Engine
	 *
	 * \param velocity New velocity
	 * \see move, PhysicsEngine
	 */
	inline void setVelocity(float x, float y) { m_velocity.x = x; m_velocity.y = y; }

	/*! Set the origin
	 * The origin of an object defines the point for transformation (move, rotate)
	 * It must be relative to the top-left corner of the object
	 *
	 * \param origin New origin
	 * \see rotate, move
	 */
	inline void setOrigin(sf::Vector2f const & origin) { m_origin = origin; }

	/*! Set the position
	 *
	 * \param position, New position
	 * \see move
	 */
	inline void setPosition(float x, float y) { m_position.x = x; m_position.y = y; }

	/*! Set the position
	 *
	 * \param position, New position
	 * \see move
	 */
	inline void setPosition(sf::Vector2f const & position) { setPosition(position.x, position.y); }

	/*! Add to the current rotation the rotation
	 *
	 * \param angle New angle, in radian
	 */
	inline void setRotation(float angle) { m_rotation = angle; }

	/*! Set whether the shape is out of the screen or not
	 * This state is define by the PhysicsEngine
	 * \see PhysicsEngine
	 */
	inline void setOutOfScreen(bool outOfScreen) { m_outOfScreen = outOfScreen; }

	/*! Set the sleep state of the object
	 *
	 * \param sleep true whether you want to compute collision or false if you don't
	 * \see rotate
	 */
	inline void setSleep(bool sleep) { m_sleep = sleep; }

	/*! Determine whether we apply the gravity or not
	 *
	 * \param gravity true whether you want to apply the gravity or false if you don't
	 */
	inline void setApplyGravity(bool applyGravity) { m_applyGravity = applyGravity; }

	/*! Set the type
	 *
	 * \param type New type
	 * \see Type
	 */
	inline void setType(AShape::Type type) { m_type = type; }

	/*! Set the collision type
	 * The collision type is use to determines whether the other object collides with this object or not
	 *
	 * To initialize the collision type, you must use power of 2
	 * \code
	 * setCollisionType(4u);
	 * setCollisionType(1u << 4u);
	 * \endcode
	 *
	 * \param collisionType New collision type
	 * \see setCollisionMask, getCollisionMask
	 */
	inline void setCollisionType(std::uint32_t collisionType) { m_collisionType = collisionType; }

	/*! Set the collision mask
	 * The collision mask is use to determines wheter the other object collides with this object or not
	 * The mask must be initialize this way
	 *
	 * if you want your object to collide with both Bee and Tree
	 * \code
	 * std::uint32_t mask = Bee->getCollideType() | Tree->getCollideType();
	 * setCollisionMask(mask);
	 * \endcode
	 *
	 * \param collisionMask New collision mask
	 * \see setCollisionType, getCollisionType
	 */
	inline void setCollisionMask(std::uint32_t collisionMask) { m_collisionMask = collisionMask; }

	/*! Set the owner */
	inline void setGameObject(AGameObjectBase * gameObject) { m_gameObject = gameObject; }

	/*! Determine wheter the object is colliding or not
	 *
	 * \param collisionType The collision type to compare
	 */
	inline bool isColliding(AShape * shape) { return (m_collisionMask & shape->getCollisionType()); }

	/*! Determine wheter the object is the same type or not
	 *
	 * \param type The type to compare
	 */
	inline bool isType(AShape::Type type) { return type == m_type; }

	/*! Add to the current velocity
	 *
	 * \param velocity The velocity to add
	 */
	inline void addVelocity(sf::Vector2f const & velocity) { addVelocity(velocity.x, velocity.y); }
	inline void addVelocity(float x, float y) { m_velocity.x += x; m_velocity.y += y; }

	/*! Get the velocity */
	inline sf::Vector2f const & getVelocity(void) const { return m_velocity; }

	/*! Get the origin */
	inline sf::Vector2f const & getOrigin(void) const { return m_origin; }

	/*! Get the position */
	inline sf::Vector2f const & getPosition(void) const { return m_position; }

	/*! Get the rotation */
	inline float getRotation(void) const { return m_rotation; }

	/*! Get whether the shape is out of the screen or not */
	inline bool isOutOfScreen(void) const { return m_outOfScreen; }

	/*! Get the sleep state of the object */
	inline bool getSleep(void) const { return (m_sleep || m_outOfScreen); }

	/*! Get the gravity state of the object */
	inline bool getApplyGravity(void) const { return m_applyGravity; }

	/*! Get the type */
	inline AShape::Type getType(void) const { return m_type; }

	/*! Get the collision type */
	inline std::uint32_t getCollisionType(void) const { return m_collisionType; }

	/*! Get the colision mask */
	inline std::uint32_t getCollisionMask(void) const { return m_collisionMask; }

	/*! Get the owner */
	inline AGameObjectBase * getGameObject(void) const { return m_gameObject; }

	/*! Rotate the shape
	 * Add the angle to the current rotation
	 *
	 * \param angle The angle to rotate
	 * \see setRotation
	 */
	inline void rotate(float angle) { m_rotation += angle; }

	/*! Apply the velocity computed by the PhysicsEngine
	 *
	 * \param deltatime The current deltatime
	 * \see PhysicsEngine
	 */
	virtual void update(void);

	/*! Use to draw debug information
	 */
	virtual void debugDraw(sf::RenderTarget & render);

	/*! Get the global bounds
	 * The AABB is recomputed at each rotation
	 *
	 * \see rotate
	 */
	virtual sf::Rect<float> const & getGlobalBounds(void) const = 0;

	/*! Get the center of the object
	 * The center is recomputed at each rotation
	 *
	 * \see rotate
	 */
	virtual sf::Vector2f const & getBaryCenter(void) const = 0;

	// Only used by the physics engine
	/*! Apply the deltatime to the shape
	 * The deltatime is given by the PhysicsEngine
	 */
	void updateVelocity(float deltatime);

	void resetVelocity(void);

	void addEngineVelocity(sf::Vector2f const & velocity);
	void addEngineVelocity(float x, float y);
	sf::Vector2f const & getEngineVelocity(void) const { return m_engineVelocity; }
	void setEngineVelocity(float x, float y);

	sf::Vector2f const & getPreviousPosition(void) const { return m_previousPosition; }
	sf::Vector2f const & getPreviousCenter(void) const { return m_previousCenter; }

	void setRenderPosition(sf::Vector2f const & renderPosition) { m_renderPosition = renderPosition; }
	void setRenderCenter(sf::Vector2f const & renderCenter) { m_renderCenter = renderCenter; }
	sf::Vector2f const & getRenderPosition(void) const { return m_renderPosition; }
	sf::Vector2f const & getRenderCenter(void) const { return m_renderCenter; }
	void registerPreviousPosition(void) { m_previousPosition = m_position;}
	void registerPreviousCenter(void) { m_previousCenter = getBaryCenter();}

protected:
	void drawCross(sf::RenderTarget & render, sf::Vector2f const & position, sf::Color const & color);

	/*! Compute the shape once per frame */
	virtual void computeShape(void) = 0;

private:
	sf::Vector2f			m_velocity;
	sf::Vector2f			m_engineVelocity;
	sf::Vector2f			m_origin;
	sf::Vector2f			m_position;
	sf::Vector2f			m_previousPosition;
	sf::Vector2f			m_renderPosition;
	sf::Vector2f			m_previousCenter;
	sf::Vector2f			m_renderCenter;
	float					m_rotation;
	bool					m_sleep;
	bool					m_applyGravity;
	bool					m_outOfScreen;
	Type					m_type;
	std::uint32_t			m_collisionType;
	std::uint32_t			m_collisionMask;
	AGameObjectBase *		m_gameObject;

	/*! Move the object
	 * This function adds to the current position of the object
	 *
	 * \param translation The offset vector
	 * \see setPosition
	 */
	inline void move(sf::Vector2f const & translation) { m_position.x += translation.x; m_position.y += translation.y; }

};

#endif

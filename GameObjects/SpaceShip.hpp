#ifndef SPACESHIP_HPP
# define SPACESHIP_HPP

# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "SmokeSystem.hpp"
# include "IPlaceable.hpp"

class CircleShape;

class SpaceShip : public AGameObject<GameObjectType::SpaceShip>, public IPlaceable
{
public:
	enum SpaceShipEvents
	{
		Flying,
		Broken
	};

	SpaceShip(SpaceShipEvents event);
	virtual ~SpaceShip(void) = default;

	void setNextEvent(SpaceShipEvents event);
	void setPosition(sf::Vector2f const & position);
	sf::Vector2f const & getPosition(void) const;
	sf::Vector2f getSize(void) const;

	void move(sf::Vector2f const & translation);
	void update(sf::Time frameTime);
	void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	SmokeSystem					m_smoke;
	SpaceShipEvents				m_event;
	CircleShape *				m_box;

};

#endif

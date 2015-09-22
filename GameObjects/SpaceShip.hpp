#ifndef SPACESHIP_HPP
# define SPACESHIP_HPP

# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>

# include "AGameObject.hpp"
# include "SmokeSystem.hpp"
# include "IPlaceable.hpp"

class SpaceShip : public AGameObject<GameObjectType::SpaceShip>, public IPlaceable
{
public:
	enum SpaceShipEvents
	{
		Flying,
		Broken
	};

	SpaceShip(SpaceShipEvents event);// statr animation
	virtual ~SpaceShip(void) = default;

	void setNextEvent(SpaceShipEvents event);
	void setPosition(sf::Vector2f const & position);

	void update(sf::Time frameTime);
	void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_flyingAnimation;
	octo::CharacterAnimation	m_brokenAnimation;
	SmokeSystem					m_smoke;
};

#endif

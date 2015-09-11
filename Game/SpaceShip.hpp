#ifndef SPACESHIP_HPP
# define SPACESHIP_HPP

# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>

# include "AGameObject.hpp"

class SpaceShip : public AGameObject<GameObjectType::SpaceShip>
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
	sf::Vector2f getSize(void) const;
	sf::Vector2f const & getPosition(void) const;

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render) const;

private:
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_flyingAnimation;
	octo::CharacterAnimation	m_brokenAnimation;

};

#endif

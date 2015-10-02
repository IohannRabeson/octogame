#ifndef WELL_HPP
# define WELL_HPP

# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "IPlaceable.hpp"

class Well : public AGameObject<GameObjectType::Well>, public IPlaceable
{
public:
	Well(void);
	virtual ~Well(void) = default;

	void setPosition(sf::Vector2f const & position);
	sf::Vector2f const & getPosition(void) const;

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;

};

#endif

#ifndef MONOLITHSTEP_HPP
# define MONOLITHSTEP_HPP

# include <AnimatedSprite.hpp>
# include "ResourceDefinitions.hpp"

class MonolithStep
{
public:
	MonolithStep(ResourceKey const & key, sf::Vector2f const & position, sf::Vector2f const & scale);
	virtual ~MonolithStep(void) = default;

	void addMapOffset(float x, float y);
	void setPosition(sf::Vector2f const & position);
	void update(sf::Time frametime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;

};

#endif

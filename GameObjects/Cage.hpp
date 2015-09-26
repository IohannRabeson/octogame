#ifndef CAGE_HPP
# define CAGE_HPP

# include "InstanceDecor.hpp"

class Cage : public InstanceDecor
{
public:
	Cage(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Cage(void) = default;

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite		m_spriteFront;
	octo::SpriteAnimation		m_animationFront;

};

#endif

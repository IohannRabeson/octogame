#ifndef FIRECAMP_HPP
# define FIRECAMP_HPP

# include "InstanceDecor.hpp"
# include "SmokeSystem.hpp"

class Firecamp : public InstanceDecor
{
public:
	Firecamp(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Firecamp(void) = default;

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	void setupSmoke(sf::Vector2f const & position);
	SmokeSystem					m_smoke;
	octo::AnimatedSprite		m_spriteFire;
	octo::SpriteAnimation		m_animationFire;

};

#endif

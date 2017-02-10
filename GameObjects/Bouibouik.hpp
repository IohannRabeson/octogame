#ifndef BOUIBOUIK_HPP
# define BOUIBOUIK_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class Bouibouik : public SimpleObject, public AGameObject<GameObjectType::Bouibouik>
{
public:
	Bouibouik(void);
	virtual ~Bouibouik(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite		m_spriteFront;
	octo::SpriteAnimation		m_animationFront;
	SmokeSystem					m_smoke;
	SmokeSystem					m_smoke2;

	void setupSmoke(void);
};

#endif

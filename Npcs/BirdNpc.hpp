#ifndef BIRD_HPP
# define BIRD_HPP

# include "ANpc.hpp"

class SkyCycle;

class BirdNpc : public ANpc, public AGameObject<GameObjectType::BirdNpc>
{
public:
	BirdNpc(void);
	virtual ~BirdNpc(void) = default;

	virtual void setup(void);
	virtual void setPosition(sf::Vector2f const & position);
	virtual float getHeight(void) const { return 0.f; }
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void update(sf::Time frametime);
	virtual void updatePhysics(void);

private:
	bool			m_animationEnd;
	sf::Vector2f	m_startPosition;
	sf::Vector2f	m_flySpeed;
	float			m_speedLimit;
};

#endif

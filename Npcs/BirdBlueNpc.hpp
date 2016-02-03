#ifndef BIRDBLUE_HPP
# define BIRDBLUE_HPP

# include "ANpc.hpp"

class SkyCycle;

class BirdBlueNpc : public ANpc, public AGameObject<GameObjectType::BirdBlueNpc>
{
public:
	BirdBlueNpc(void);
	virtual ~BirdBlueNpc(void) = default;

	virtual void setup(void);
	virtual void setPosition(sf::Vector2f const & position);
	virtual float getHeight(void) const { return 0.f; }
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

private:
	bool			m_startTimer;
	bool			m_animationEnd;
	sf::Vector2f	m_startPosition;
	sf::Vector2f	m_flySpeed;
};

#endif

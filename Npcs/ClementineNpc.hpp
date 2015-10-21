#ifndef CLEMENTINENPC_HPP
# define CLEMENTINENPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class ClementineNpc : public ANpc, public AGameObject<GameObjectType::ClementineNpc>
{
public:
	ClementineNpc(void);
	virtual ~ClementineNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const { return 0.f; }

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);
	virtual bool canSpecial1(void) const;

private:
	sf::Time	m_timer;
	bool		m_canSpecial;
};

#endif
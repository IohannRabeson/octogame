#ifndef FANNYNPC_HPP
# define FANNYNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class FannyNpc : public ANpc, public AGameObject<GameObjectType::FannyNpc>
{
public:
	FannyNpc(void);
	virtual ~FannyNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const { return 0.f; }
	virtual void startBalle(void);

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

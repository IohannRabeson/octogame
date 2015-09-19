#ifndef FRANFRANNPC_HPP
# define FRANFRANNPC_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class SkyCycle;

class FranfranNpc : public ANpc, public AGameObject<GameObjectType::FranfranNpc>
{
public:
	FranfranNpc(void);
	virtual ~FranfranNpc(void) = default;

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
	bool		m_canSmoke;
	SmokeSystem	m_smoke;
	SmokeSystem m_puff;
	sf::Time	m_puffTimer;
	sf::Time	m_puffTimerMax;
};

#endif

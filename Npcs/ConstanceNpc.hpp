#ifndef CONSTANCENPC_HPP
# define CONSTANCENPC_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class SkyCycle;

class ConstanceNpc : public ANpc, public AGameObject<GameObjectType::ConstanceNpc>
{
public:
	ConstanceNpc(void);
	virtual ~ConstanceNpc(void) = default;

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

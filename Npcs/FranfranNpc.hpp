#ifndef FRANFRANNPC_HPP
# define FRANFRANNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class FranfranNpc : public ANpc, AGameObject<GameObjectType::FranfranNpc>
{
public:
	FranfranNpc(void);
	virtual ~FranfranNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);
	virtual bool canSpecial1(void) const;

private:
	sf::Time	m_timer;
	bool		m_canSmoke;
};

#endif

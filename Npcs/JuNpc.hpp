#ifndef JUNPC_HPP
# define JUNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class JuNpc : public ANpc, public AGameObject<GameObjectType::JuNpc>
{
public:
	JuNpc(void);
	virtual ~JuNpc(void) = default;

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

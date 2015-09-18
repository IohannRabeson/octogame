#ifndef TURBANNPC_HPP
# define TURBANNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class TurbanNpc : public ANpc, public AGameObject<GameObjectType::TurbanNpc>
{
public:
	TurbanNpc(void);
	virtual ~TurbanNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const { return 0.f; }

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);
};

#endif

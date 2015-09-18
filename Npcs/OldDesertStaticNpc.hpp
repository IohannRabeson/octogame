#ifndef OLDDESERTSTATICNPC_HPP
# define OLDDESERTSTATICNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class OldDesertStaticNpc : public ANpc, public AGameObject<GameObjectType::OldDesertStaticNpc>
{
public:
	OldDesertStaticNpc(void);
	virtual ~OldDesertStaticNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const { return 0.f; }

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

private:
};

#endif

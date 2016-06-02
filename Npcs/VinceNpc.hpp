#ifndef VINCE_HPP
# define VINCE_HPP

# include "ANpc.hpp"

class SkyCycle;

class VinceNpc : public ANpc, public AGameObject<GameObjectType::VinceNpc>
{
public:
	VinceNpc(void);
	virtual ~VinceNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	virtual void setupMachine(void);
};

#endif
